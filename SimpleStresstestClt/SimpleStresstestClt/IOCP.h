#pragma once

#include "Network.h"
#include <atomic>
#include <thread>

class IOCP : public Network
{
public:
	IOCP();

	virtual ~IOCP()
	{

	}
private:
	HANDLE hIocp;

	std::atomic_bool bInit = false;

	// Inherited via Network
	virtual bool Join(SOCKET sock, INetworkClt* CltHandle) override;
};