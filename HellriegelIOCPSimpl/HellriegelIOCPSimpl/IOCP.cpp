#include "pch.h"
#include "IOCP.h"

#include <cassert>

#include <WS2tcpip.h>
#include <MSWSock.h>

class Listener : public ICompletionKey
{
public:
	Listener( HANDLE hIocp, std::string strListenPort)
		: _listenPort(std::move(strListenPort)), ICompletionKey( hIocp, _sockctxListen._socket )
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

		if ( 0 != ::getaddrinfo(nullptr, strListenPort.c_str(), &hints, &addrlocal)) {
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

		auto nRet = ::bind( _sockctxListen._socket, addrlocal->ai_addr, (int) addrlocal->ai_addrlen );
		if (SOCKET_ERROR == nRet)
		{
			LOG_FN(" bind() failed: ", WSAGetLastError());
			return;
		}

		nRet = ::listen(_sockctxListen._socket, 5);
		if (SOCKET_ERROR == nRet)
		{
			LOG_FN( "listen() failed: ", WSAGetLastError() );
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
			LOG_FN( "failed to load AcceptEx: ", WSAGetLastError() );
			return;
		}

		bInit = true;
	}

	bool AsyncAccept( IOAccept& acceptIO )
	{
		assert( INVALID_SOCKET != _sockctxListen._socket );
		if (INVALID_SOCKET == acceptIO.socketToAccept._socket)
		{
			LOG_FN(" Invalid socketToAccept  ");
			return false;
		}

		const int nMaxBuffer = sizeof(acceptIO.buffer);
		const int addressLen = sizeof(SOCKADDR_STORAGE) + 16;
		
		auto nRet = _fnAcceptEx( _sockctxListen._socket, acceptIO.socketToAccept._socket,
			(LPVOID)acceptIO.buffer, nMaxBuffer - (2*addressLen),
			addressLen, addressLen, acceptIO.GetRecevedBytePtr(),
			static_cast<LPOVERLAPPED>(&acceptIO));

		if (SOCKET_ERROR == nRet && ERROR_IO_PENDING != WSAGetLastError())
		{
			LOG_FN( "AcceptEx() failed: ", WSAGetLastError() );
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
private:
};


bool IOCP::IsInvalidHandle(HANDLE handle)
{
	return (handle == INVALID_HANDLE_VALUE || handle == NULL);
}

bool IOCP::Listen(std::string strPort)
{
	auto listener = NewKey<Listener>(hIOCP,strPort);
	Listener* pCur = dynamic_cast<Listener*>( listener.get() );

	if (nullptr == pCur)
	{
		LOG_FN( "Failed to create Listener" );
		Delete(listener.get());
		return false;
	}

	return ReservAccept(*pCur);	
}

bool IOCP::ReservAccept(Listener& listener)
{
	auto ioAccept = NewIO< IOAccept >();
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

IOCP::IOCP()
	: hIOCP(::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0))
{
	if (IsInvalidHandle(hIOCP))
	{
		LOG_FN( "Failed to initialize IOCP handle: ", WSAGetLastError() );
		return;
	}

	//std::generate(vecThreads.begin(), vecThreads.end(), [&] { return std::thread{ &IOCP::WorkerThread, this }; });
	for (int i = 0; i < MAX_WORKER; ++i)
		vecThreads.emplace_back( std::thread{ &IOCP::WorkerThread, this });

	bInit = true;
}

IOCP::~IOCP()
{
	std::vector<BOOL> bPostResult(MAX_WORKER);
	std::generate(bPostResult.begin(), bPostResult.end(), [&] { return ::PostQueuedCompletionStatus(hIOCP, 0, NULL, NULL); });

	for (auto& cur : vecThreads) {
		cur.join();
	}
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
	AutoReset< decltype(bRunning) > runningReset( &bRunning, true );
	while (true)
	{
		ICompletionKey* completionKey(nullptr);
		LPOVERLAPPED lpOverlapped(nullptr);
		DWORD dwIOSize(0);
		BOOL bSuccess = ::GetQueuedCompletionStatus( hIOCP, &dwIOSize, (PULONG_PTR)&completionKey, &lpOverlapped, INFINITE );
		if (FALSE == bSuccess)
		{
			LOG_FN( std::this_thread::get_id(), " GetQueuedCompletionStatus() failed with error : ", WSAGetLastError() );
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


		if (ioCur->GetMyT() == EIO_ACCEPT)
		{
			IOAccept* pAccept = dynamic_cast<IOAccept*>(ioCur);
			if ( nullptr == pAccept )
			{
				LOG_FN("SYSTEM_ERROR: non IOAccept returns EIO_ACCPET ");
				continue;
			}

			Listener* listener = dynamic_cast<Listener*>(completionKey);
			if (nullptr == listener)
			{
				LOG_FN("SYSTEM_ERROR: someone else reserved acceptex ");
				continue;
			}
			ReservAccept(*listener); //Reserv next accept

			SOCKET_CTX sockctx( std::move( pAccept->socketToAccept )); //create new client ctx from this

			//remove current ctx

			continue;
		}
		completionKey, ioCur, dwIOSize;

		/*
		PIOCtx ioNew = _contIOCtx._New< IORead >();
		Delete(ioNew.get());		
		*/
	}
}

