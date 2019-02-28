#pragma once

#include "Network.h"

#include <atomic>
#include <chrono>

class Client : public INetworkClt
{
public:
	//every client only connect with single server, if possible to make it multiple server(possible server) but in this case.
	Client( Network& network, std::string strIP, short _strPort );
	virtual ~Client();

	//If unable to send, save to send queue for next possible
	void InvokeSend( char const* buffer, size_t len ); 
private:
	// Invoke Connect after failed to connect or constructor
	void InvokeConnect();
	SOCKET _socket;
	std::string		_strIP;
	short			_strPort;

	struct {
		std::chrono::system_clock::time_point tpReserved;
		std::vector< unsigned char > Data;
	};

	std::atomic_bool bPossible;
	std::atomic_bool bInit = false;
};

