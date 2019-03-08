#pragma once

class INetworkClt
{
public:
	//don't made below like this, just in case in turst provide self Socket. If else, another clientcode possible to ::closesocket()
	//best practice Network::Join( SOCKET, INetworkClt )
	//virtual SOCKET GetSocket() = 0; 
};

class Network
{
public:
	Network();
	virtual ~Network();

	virtual bool Join(SOCKET sock, INetworkClt* CltHandle ) = 0;
};

