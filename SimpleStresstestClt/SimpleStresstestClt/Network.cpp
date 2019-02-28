#include "pch.h"
#include "Network.h"

#pragma comment( lib, "ws2_32.lib" )

class WSAInit
{
	WSAInit()
	{
		auto ret = ::WSAStartup(MAKEWORD(2, 2), &_wsaData);
		if (0 != ret)
		{
			LOG_FILELINE_FN( " WSAStartup() failed with error: ", ret , ", WSA-> ", WSAGetLastError() );
			::exit(-1);
		}
	}

	~WSAInit()
	{
		auto ret = ::WSACleanup();
		if (0 != ret)
		{
			LOG_FILELINE_FN("WSACleanup() failed with error:", ret, ", WSA-> ", WSAGetLastError());
			__DO_NOTHING__; //
		}
	}

public:

	static WSAInit& GetInstance() {
		static WSAInit _inst;
		return _inst;
	}
private:
	WSADATA _wsaData;

}& wsaInit = WSAInit::GetInstance();


Network::Network()
{
}


Network::~Network()
{
}
