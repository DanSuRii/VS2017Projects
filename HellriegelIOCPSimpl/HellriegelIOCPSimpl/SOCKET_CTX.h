#pragma once

struct SOCKET_CTX
{
	SOCKET_CTX();
	~SOCKET_CTX();

	SOCKET_CTX(const SOCKET_CTX&) = delete;
	SOCKET_CTX operator=(const SOCKET_CTX&) = delete;
	SOCKET_CTX(SOCKET_CTX&&);
	SOCKET	_socket = INVALID_SOCKET;

	inline bool IsInit() { return bInit; }
private:
	std::atomic_bool bInit = false;
};