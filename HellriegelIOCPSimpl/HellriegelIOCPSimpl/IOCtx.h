#pragma once

#include <WinSock2.h>
#include "IBufHandle.h"

enum EIOCTX
{
	EIO_ACCEPT
	, EIO_READ
	, EIO_WRITE
	, EIO_CNT
};

class IOCtx : public OVERLAPPED
{
public:

	virtual ~IOCtx()
	{

	}
	virtual void Dispose() = 0;
	virtual EIOCTX GetMyT() = 0;

protected:
};

using PIOCtx = std::shared_ptr< IOCtx >;

template< EIOCTX cur, class Derived, bool bIBufHandle >
class IMPL_IOCTX : public IOCtx
{
public:
	IMPL_IOCTX() = delete;
	IMPL_IOCTX(const IMPL_IOCTX&) = delete;
	IMPL_IOCTX& operator=(const IMPL_IOCTX&) = delete;
	virtual void DO_NOT_INEHRIT_FROM_HERE() = 0; //normally impossible, but notify
};

//dont use buff version, its normal
template< EIOCTX cur, class Derived >
class IMPL_IOCTX<cur, Derived, /* bIBufHandle= */false> : public IOCtx
{	
public:
	using _MyT = Derived;
	virtual inline EIOCTX GetMyT() { return cur; }

	IMPL_IOCTX()
	{
	}

	virtual ~IMPL_IOCTX()
	{

	}
};

//use buff version
template< EIOCTX cur, class Derived >
class IMPL_IOCTX<cur, Derived, /* bIBufHandle= */true> : public IOCtx
{
public:
	using _MyT = Derived;
	virtual inline EIOCTX GetMyT() { return cur; }

	IMPL_IOCTX(PIBufHandle&& pBufHandle);
	IMPL_IOCTX(const IMPL_IOCTX&) = delete;
	IMPL_IOCTX& operator=(const IMPL_IOCTX&) = delete;

	virtual ~IMPL_IOCTX()
	{

	}

	//Wrapper for access buff
	inline char* GetBuf() {
		//assert(nullptr != _bufHandle);
		if (nullptr == _bufHandle) {
			LOG_FN("SYSTEM_ERROR: BuffHandle impossible to ");
			_CrtDbgBreak();
			return nullptr;
		}

		return _bufHandle->GetBufPtr();
	}
	inline size_t GetBufLen() {
		//assert(nullptr != _bufHandle);
		if (nullptr == _bufHandle) {
			LOG_FN("SYSTEM_ERROR: BuffHandle impossible to ");
			_CrtDbgBreak();
			return 0;
		}
		return _bufHandle->GetBufferSize();
	}


protected:
	//this member impossible to null
	PIBufHandle _bufHandle;

private:
	IMPL_IOCTX() = delete;
};


template< class TDerived ,EIOCTX cur, bool bIBufHandle = false >
class Concrete_IOCTX : public IMPL_IOCTX< cur, TDerived, bIBufHandle >
{
public:
	using _IMPL_IOCTX_T = IMPL_IOCTX< cur, TDerived, bIBufHandle >;
	
	template< class ... Args >
	Concrete_IOCTX(Args&& ...args)
		: _IMPL_IOCTX_T( std::forward<Args>(args) ... )
	{

	}
};

class IOAccept : public Concrete_IOCTX<IOAccept, EIO_ACCEPT, true>
{
public:
	//using _PartenType = Concrete_IOCTX<IOAccept, EIO_ACCEPT, true>;
	IOAccept(PIBufHandle&& bufHandle);
	virtual ~IOAccept()
	{
	}

	SOCKET_CTX socketToAccept;

	inline bool IsInit() { return bInit; }
	LPDWORD		GetRecevedBytePtr() { return &dwRecevedBytes; }

private:	
	DWORD		dwRecevedBytes;

	std::atomic_bool bInit = false;
	
};

class IORead : public Concrete_IOCTX<IORead, EIO_READ>
{
public:
	IORead()
	{

	}
private:
};

class IOWrite : public Concrete_IOCTX<IOWrite, EIO_WRITE>
{
public:
	IOWrite()
	{

	}
private:
};

#if false
template<>
class Concrete_IOCTX< EIO_ACCEPT > : public IMPL_IOCTX< EIO_ACCEPT, Concrete_IOCTX< EIO_ACCEPT > >
{
public:
	struct SOCKET_CTX
	{
		SOCKET sock;
	};
};

#define DECL_IOCTX( x ) template<> class Concrete_IOCTX< x > : public IMPL_IOCTX< x, Concrete_IOCTX< x > >
DECL_IOCTX(EIO_WRITE)
{

};

DECL_IOCTX(EIO_READ)
{

};

#endif // false

