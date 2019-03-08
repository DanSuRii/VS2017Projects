#include "pch.h"
#include "IOCP.h"

#include <cassert>

#include <WS2tcpip.h>
#include <MSWSock.h>

class Listener : public ICompletionKey
{
public:
	Listener(HANDLE hIocp, std::string strListenPort)
		: _listenPort(std::move(strListenPort)), ICompletionKey(hIocp, _sockctxListen._socket)
	{
		LINGER lingerStruct;
		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0;

		struct addrinfo hints = { 0 };
		struct addrinfo *addrlocal = nullptr;

		// Resolve the interface

		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_IP;

		if (0 != ::getaddrinfo(nullptr, strListenPort.c_str(), &hints, &addrlocal)) {
			LOG_FN(" getaddrinfo() failed with error: ", WSAGetLastError());
			return;
		}

		if (nullptr == addrlocal)
		{
			LOG_FN(" getaddrinfo() failed to resolve/convert the interface ");
			return;
		}

		RAII addrgrd([&] { ::freeaddrinfo(addrlocal); });

		//msdn https://docs.microsoft.com/en-us/windows/desktop/api/winsock2/nf-winsock2-socket
		//sock wiull be create OVERLAPPED

		auto nRet = ::bind(_sockctxListen._socket, addrlocal->ai_addr, (int)addrlocal->ai_addrlen);
		if (SOCKET_ERROR == nRet)
		{
			LOG_FN(" bind() failed: ", WSAGetLastError());
			return;
		}

		nRet = ::listen(_sockctxListen._socket, 5);
		if (SOCKET_ERROR == nRet)
		{
			LOG_FN("listen() failed: ", WSAGetLastError());
			return;
		}

		GUID acceptex_guid = WSAID_ACCEPTEX;
		nRet = WSAIoctl(
			_sockctxListen._socket,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&acceptex_guid,
			sizeof(acceptex_guid),
			&_fnAcceptEx,
			sizeof(_fnAcceptEx),
			&dwFnBytes,
			NULL,
			NULL
		);
		if (SOCKET_ERROR == nRet)
		{
			LOG_FN("failed to load AcceptEx: ", WSAGetLastError());
			return;
		}

		bInit = true;
	}

	bool AsyncAccept(IOAccept& acceptIO)
	{
		assert(INVALID_SOCKET != _sockctxListen._socket);
		if (INVALID_SOCKET == acceptIO.socketToAccept._socket)
		{
			LOG_FN(" Invalid socketToAccept  ");
			return false;
		}

		const int nMaxBuffer = acceptIO.GetBufLen();
		const int addressLen = sizeof(SOCKADDR_STORAGE) + 16;

		auto nRet = _fnAcceptEx(_sockctxListen._socket, acceptIO.socketToAccept._socket,
			(LPVOID)acceptIO.GetBuf(), nMaxBuffer - (2 * addressLen),
			addressLen, addressLen, acceptIO.GetRecevedBytePtr(),
			static_cast<LPOVERLAPPED>(&acceptIO));

		if (SOCKET_ERROR == nRet && ERROR_IO_PENDING != WSAGetLastError())
		{
			LOG_FN("AcceptEx() failed: ", WSAGetLastError());
			return false;
		}

		return true;
	}

	~Listener()
	{

	}

	inline bool IsInit() { return bInit; }
private:
	LPFN_ACCEPTEX		_fnAcceptEx;
	DWORD				dwFnBytes = 0;

	std::atomic_bool bInit = false;
	std::string		_listenPort;
	SOCKET_CTX				_sockctxListen;
};

class CltCtx : public ICompletionKey
{
public:
	CltCtx(SOCKET_CTX&& rhs, HANDLE hIocp)
		: _sockCtx(std::move(rhs)), ICompletionKey(hIocp, _sockCtx._socket)
	{

	}
private:
	SOCKET_CTX _sockCtx;
};


bool IOCP::IsInvalidHandle(HANDLE handle)
{
	return (handle == INVALID_HANDLE_VALUE || handle == NULL);
}

bool IOCP::Listen(std::string strPort)
{
	auto listener = NewKey<Listener>(hIOCP, strPort);
	Listener* pCur = dynamic_cast<Listener*>(listener.get());

	if (nullptr == pCur)
	{
		LOG_FN("Failed to create Listener");
		Delete(listener.get());
		return false;
	}

	return ReservAccept(*pCur);
}

bool IOCP::ReservAccept(Listener& listener)
{
	auto ioAccept = NewIO< IOAccept >(_bufferPool.GetNewBuf());
	IOAccept* pIoAccept = dynamic_cast<IOAccept*>(ioAccept.get());

	if (nullptr == pIoAccept)
	{
		LOG_FN("Failed to create new Accpet");
		Delete(ioAccept.get());
		return false;
	}

	listener.AsyncAccept(*pIoAccept);

	return false;
}

IOCP::IOCP(IBufferPool & bufferPool) :
	_bufferPool(bufferPool)
	, hIOCP(::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0))
{
	if (IsInvalidHandle(hIOCP))
	{
		LOG_FN("Failed to initialize IOCP handle: ", WSAGetLastError());
		return;
	}

	//std::generate(vecThreads.begin(), vecThreads.end(), [&] { return std::thread{ &IOCP::WorkerThread, this }; });
	for (int i = 0; i < MAX_WORKER; ++i)
		vecThreads.emplace_back(std::thread{ &IOCP::WorkerThread, this });

	bInit = true;
}

IOCP::~IOCP()
{
	std::vector<BOOL> bPostResult(MAX_WORKER);
	std::generate(bPostResult.begin(), bPostResult.end(), [&] { return ::PostQueuedCompletionStatus(hIOCP, 0, NULL, NULL); });

	for (auto& cur : vecThreads) {
		cur.join();
	}

	//Make All Container empty

}


void IOCP::Delete(ICompletionKey * ptr)
{
	_contCompl._Delete(ptr);
}

void IOCP::Delete(IOCtx * ptr)
{
	_contIOCtx._Delete(ptr);
}

void IOCP::WorkerThread()
{
	AutoReset< decltype(bRunning) > runningReset(&bRunning, true);
	while (true)
	{
		ICompletionKey* completionKey(nullptr);
		LPOVERLAPPED lpOverlapped(nullptr);
		DWORD dwIOSize(0);
		BOOL bSuccess = ::GetQueuedCompletionStatus(hIOCP, &dwIOSize, (PULONG_PTR)&completionKey, &lpOverlapped, INFINITE);
		if (FALSE == bSuccess)
		{
			LOG_FN(std::this_thread::get_id(), " GetQueuedCompletionStatus() failed with error : ", WSAGetLastError());
		}

		if (nullptr == completionKey)
		{
			LOG_FN(std::this_thread::get_id(), " >> Recevied exit ");
			break;
		}

		IOCtx* ioCur = static_cast<IOCtx*>(lpOverlapped);
		if (nullptr == ioCur)
		{
			LOG_FN("SYSTEM_ERROR: null ioctx passed ");
			continue;
		}
		//Reserve ioctx remove after this
		RAII ioGrd([&] {Delete(ioCur); });

		if (EIO_ACCEPT != ioCur->GetMyT())
		{
			if (!bSuccess || (bSuccess && 0 == dwIOSize)) {
				//client may be dropped.
				LOG_FN("Detected dropped client: ", completionKey->GetID());
				Delete(completionKey);
			}
		}

		completionKey, ioCur, dwIOSize;

		/*
		PIOCtx ioNew = _contIOCtx._New< IORead >();
		Delete(ioNew.get());
		*/
	}
}

bool IOCP::Handle(IOAccept & pAccept, Listener & listener, DWORD dwIoSize)
{
	ReservAccept(listener); //Reserv next accept

	NewKey< CltCtx >(std::move(pAccept.socketToAccept), hIOCP);

	return true;
}

bool IOCP::Handle(IORead &, CltCtx &, DWORD dwIoSize)
{
	return false;
}

bool IOCP::Handle(IOWrite &, CltCtx &, DWORD dwIoSize)
{
	return false;
}

template<class T_IO, class T_ComplKey>
bool IOCP::CheckAndCall(IOCtx* pIO, ICompletionKey* pCompletion, DWORD dwIoSize)
{
	T_IO* pIoImpl = dynamic_cast<T_IO*>(pIO);
	if (nullptr == pIoImpl)
	{
		//LOG_FN("SYSTEM_ERROR: non IOAccept returns EIO_ACCPET ");
		LOG_FN("SYSTEM_ERROR: pIO is not a type ", typeid(T_IO).name(), " real type is ", typeid(*pIO).name());
		return false;
	}

	T_ComplKey* pCompleKey = dynamic_cast<T_ComplKey*>(pCompletion);
	if (nullptr == pCompleKey)
	{
		LOG_FN("SYSTEM_ERROR: pCompletion is not a type ", typeid(T_ComplKey).name(), " real type is ", typeid(*pCompletion).name());
		return false;
	}
	return Handle(*pIoImpl, *pCompleKey, dwIoSize);
}


bool IOCP::Handle(IOCtx *ioCur, ICompletionKey * completionKey, DWORD dwIoSize)
{
	assert(nullptr != ioCur);
	assert(nullptr != completionKey);

	switch (ioCur->GetMyT())
	{
	case EIO_ACCEPT:
		return CheckAndCall< IOAccept, Listener >(ioCur, completionKey, dwIoSize);

	case EIO_READ:
		return CheckAndCall< IORead, CltCtx >(ioCur, completionKey, dwIoSize);

	case EIO_WRITE:
		return CheckAndCall< IOWrite, CltCtx >(ioCur, completionKey, dwIoSize);
	}

	LOG_FN("SYSTEM_ERROR: Unsupported IOCP was been called, IoType: ", typeid(*ioCur).name(), ", CompletionKeyType: ", typeid(*completionKey).name());

	return false;
}

