#include "pch.h"
#include "IOCP.h"

IOCP::IOCP()
{
	//std::generate(vecThreads.begin(), vecThreads.end(), [&] { return std::thread{ &IOCP::WorkerThread, this }; });
	for (int i = 0; i < MAX_WORKER; ++i)
		vecThreads.emplace_back( std::thread{ &IOCP::WorkerThread, this });
}

IOCP::~IOCP()
{
	std::vector<BOOL> bPostResult(MAX_WORKER);
	std::generate(bPostResult.begin(), bPostResult.end(), [&] { return ::PostQueuedCompletionStatus(hIOCP, 0, NULL, NULL); });

	for (auto& cur : vecThreads) {
		cur.join();
	}
}

class ICompletionKey
{

};

void IOCP::WorkerThread()
{
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


	}
}
