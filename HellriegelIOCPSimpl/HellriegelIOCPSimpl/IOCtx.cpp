#include "pch.h"
#include "IOCtx.h"

#include <cassert>
#include <memory>
#include <list>
#include <shared_mutex>

class LocalBuff : public IBufHandle
{
	// Inherited via IBufHandle
	virtual char * GetBufPtr() override
	{
		return _buffer;
	}
	virtual size_t GetBufferSize() override
	{
		return sizeof(_buffer);
	}
public:
	BYTEBUFFER _buffer;
};

template<EIOCTX cur, class Derived>
IMPL_IOCTX<cur, Derived, true>::IMPL_IOCTX(PIBufHandle && pBufHandle)
	: _bufHandle( std::move( pBufHandle ) )
{
	if (nullptr == _bufHandle)
	{
		LOG_FN(" NULLPTR come, but must be guarentee Buffer, then Localbuffer Assign Requested Object (", typeid(Derived).name(), ")");
		_bufHandle = New<LocalBuff>();
	}
}


IOAccept::IOAccept(PIBufHandle && bufHandle)
	: Concrete_IOCTX( std::forward<PIBufHandle>( bufHandle ) )
	,dwRecevedBytes(0)
{

	if (false == socketToAccept.IsInit())
	{
		LOG_FN("Init Accept Failure");
		return;
	}

	bInit = true;
}

