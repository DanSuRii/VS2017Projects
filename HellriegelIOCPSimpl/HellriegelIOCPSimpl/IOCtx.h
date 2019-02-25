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
};

template< EIOCTX cur >
class Concrete_IOCTX : public IMPL_IOCTX< cur, Concrete_IOCTX<cur> >
{

};

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