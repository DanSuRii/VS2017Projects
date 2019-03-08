#include "pch.h"
#include "Network.h"

#pragma comment( lib, "ws2_32.lib" )

class WSAInitializer
{
	WSAInitializer()
	{
		int nRet(0);
		nRet = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
		if( nRet != 0 )
		{
			LOG_FN( "SYSTEM FAULT : WSA initialization failed >> ", GetLastError() );
			::exit(-1);
		}
	}

public:
	virtual ~WSAInitializer()
	{
		::WSACleanup();
	}
	static WSAInitializer& GetInstance() {
		static WSAInitializer _inst;
		return _inst;
	}
private:
	WSADATA wsaData;
}& wsainitializer = WSAInitializer::GetInstance();


Network::Network()
{
}


Network::~Network()
{
}
