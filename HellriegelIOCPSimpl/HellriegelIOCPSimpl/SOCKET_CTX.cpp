#include "pch.h"
#include "SOCKET_CTX.h"

SOCKET_CTX::SOCKET_CTX()
{
	_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (INVALID_SOCKET == _socket)
	{
		LOG_FN("Failed to create socket : ", WSAGetLastError());
		return;
	}

	bInit = true;
}

SOCKET_CTX::~SOCKET_CTX()
{
	if (INVALID_SOCKET != _socket)
	{
		::shutdown(_socket, SD_BOTH);
		::closesocket(_socket);

		_socket = INVALID_SOCKET;
	}
}
