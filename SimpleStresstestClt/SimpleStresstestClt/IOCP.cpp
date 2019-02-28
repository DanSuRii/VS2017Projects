#include "pch.h"
#include "IOCP.h"


IOCP::IOCP()
{
	hIocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (NULL == hIocp)
	{
		LOG_FILELINE_FN( "SYSTEM_ERROR: Failed to create CreateIoCompletionPort()" );
		return;
	}

	

	bInit = true;
}

bool IOCP::Join(SOCKET sock, INetworkClt* CltHandle)
{
	auto resIocp = ::CreateIoCompletionPort((HANDLE)sock, hIocp, (ULONG_PTR)CltHandle, 0 );
	if (NULL == resIocp)
	{
		LOG_FN( "CreateIoCompletionPort() Failed to join to IOCP: ", WSAGetLastError() );
		return false;
	}

	return true;
}
