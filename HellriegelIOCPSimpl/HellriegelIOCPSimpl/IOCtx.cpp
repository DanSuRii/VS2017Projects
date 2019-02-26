#include "pch.h"
#include "IOCtx.h"

#include <memory>
#include <list>
#include <shared_mutex>




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
