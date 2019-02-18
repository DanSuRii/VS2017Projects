#ifndef __MSGS_H__
#define __MSGS_H__

#pragma pack(push, 1)

typedef int MSG_TYPE;
typedef int MSG_LEN;
typedef unsigned long long TIME_POINT;

struct Msg
{
    struct _head
    {
        MSG_TYPE   _type;
        MSG_LEN    _len;        
    };

    virtual MSG_LEN GetSize() = 0;
};

template< typename _Derived >
struct MsgImplIntf : public Msg
{
    virtual MSG_LEN GetSize() override
    {
        return sizeof( _Derived );
    }    

    //for a prevent Miss inherit
    auto CheckCast()
    {
        //it checks compiltime 
        return static_cast<_Derived*>(this);
    }
};

#define DECL_MSG(x) struct x : public MsgImplIntf<x>


// Message requested from clients

DECL_MSG( ReqLiquiditaet )
{
    TIME_POINT timeBegin;
    TIME_POINT timeEnd;
};

// Message response from Server

DECL_MSG( ResLiquiditaet )
{
    enum{ MSG_SUCCEED, MSG_FAILURE } eRes;
};

DECL_MSG( InfoLiquiditaet )
{
    static const int MAX_FILE_LEN = 50;
    char uncPath[MAX_FILE_LEN];
};



#pragma pack(pop)
#endif // __MSGS_H__