#include "pch.h"
#include "IOCtx.h"

IOAccept::IOAccept()
	: dwRecevedBytes(0)
{
	if (false == socketToAccept.IsInit())
	{
		LOG_FN( "Init Accept Failure" );
		return;
	}

	bInit = true;
}
