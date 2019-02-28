#include "pch.h"
#include "Client.h"

#include <vector>

//GenerateRandomMsg();


std::pair<WSABUF, size_t> GetNewSend()
{

}


Client::Client(Network& network)
{
	_socket = ::socket( AF_INET, SOCK_STREAM, IPPROTO_IP );
	if (INVALID_SOCKET == _socket)
	{
		LOG_FN( "Failed to initialize socket: ", WSAGetLastError() );
		return;
	}
	
	network.Join(_socket, this);

	


	std::vector< WSABUF > bufToSend;
	
	DWORD dwNumToSent(0);

	/*
	lpNumberOfBytesSent
		A pointer to the number, in bytes, sent by this call if the I/O operation completes immediately.
		Use NULL for this parameter if the lpOverlapped parameter is not NULL to avoid potentially erroneous results. This parameter can be NULL only if the lpOverlapped parameter is not NULL.
	*/
	DWORD dwFlag(0);
	OVERLAPPED overlapped;

	WSASend(_socket, bufToSend.data(), bufToSend.size(), NULL, dwFlag, &overlapped, NULL);



	bInit = true;
}

Client::~Client()
{
}
