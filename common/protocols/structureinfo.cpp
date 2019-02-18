using TIME_POINT = unsigned long long;


template< class T >
class TypeInfoHolder
{
public:

};

template< class C >
class RegisteredMemberPos
{
public:
        
};

template< class C, class T >
class MemberTypeInfoHolder
{
public:
    
};

template< class C >
class Serialier
{
public:
    std::pair< char*, size_t > Serialize( C& rhs )
    {
        

        MemberTypeInfoHolder< C, n >::GetPtr();
        MemberTypeInfoHolder< C, n >::GetLen();
    }
    bool Deserialize( std::pair< char*, size_t > bufInfo, C& toOut );    
};

#define DECLMSG_BEGIN(x) struct x { using _MyT = x;
#define DECLMSG_END  };

#define DECL_MEMBER( TYPENAME, M_NAME ) TYPENAME M_NAME; MemberTypeInfoHolder< _MyT, TYPENAME >

DECLMSG_BEGIN(ReqLiquiditaet)
    DECL_MEMBER( TIME_POINT, timeBegin )
DECLMSG_END

struct ReqLiquiditaet
{
    TIME_POINT timeBegin; 
    TypeInfoHolder<TIME_POINT>  TypeInfoHoldertimebegin()
    {
        return TypeInfoHolder<TIME_POINT>();
    }

    MemberTypeInfoHolder< decltype<> >
    TIME_POINT timeEnd;
};

// Message response from Server

struct ResLiquiditaet 
{
    enum{ MSG_SUCCEED, MSG_FAILURE } eRes;
};
