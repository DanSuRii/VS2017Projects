#include "pch.h"
#include "ICompletionKey.h"

#include <cassert>

decltype(CKID::generator) CKID::generator = 0;

CKID::CKID()
	: _id( ++generator )
{

}

ICompletionKey::ICompletionKey(HANDLE hIocp, SOCKET sockToJoin)
	: _hIocp(hIocp)
{
	_idClt = -1;

	assert(INVALID_HANDLE_VALUE != _hIocp, "INVALID_HANDLE_VALUE");
	assert(NULL != hIocp, " HANDLE is NULL ");
	if (INVALID_SOCKET == sockToJoin)
	{
		LOG_FN( "Invalid Socket" );
		return;
	}

	HANDLE hIocpRes = ::CreateIoCompletionPort((HANDLE)sockToJoin, hIocp, (ULONG_PTR)this, 0);
	if (NULL == hIocpRes)
	{
		LOG_FN( "join to CreateIoCompletionPort() failed: ", GetLastError() );
		return;
	}

	bInit = true;
}
