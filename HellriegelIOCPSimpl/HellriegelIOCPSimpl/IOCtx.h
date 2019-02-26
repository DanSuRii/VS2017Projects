#pragma once

#include <WinSock2.h>

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
};

using PIOCtx = std::shared_ptr< IOCtx >;

template< EIOCTX cur, class Derived >
class IMPL_IOCTX : public IOCtx
{	
public:
	using _MyT = Derived;
	virtual inline EIOCTX GetMyT() { return cur; }

	virtual ~IMPL_IOCTX()
	{

	}
};

template< EIOCTX cur >
class Concrete_IOCTX : public IMPL_IOCTX< cur, Concrete_IOCTX<cur> >
{
public:
};

class IOAccept : public Concrete_IOCTX<EIO_ACCEPT>
{
public:
	IOAccept();
	virtual ~IOAccept()
	{

	}

	SOCKET_CTX socketToAccept;
	READBUFFER buffer;

	inline bool IsInit() { return bInit; }
	LPDWORD		GetRecevedBytePtr() { return &dwRecevedBytes; }
private:	
	DWORD		dwRecevedBytes;

	std::atomic_bool bInit = false;
};

class IORead : public Concrete_IOCTX<EIO_READ>
{
public:
private:

};

class IOWrite : public Concrete_IOCTX<EIO_WRITE>
{
public:
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
