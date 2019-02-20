#pragma once
#include <thread>
#include <vector>
#include <algorithm>

#include "Network.h"

class IOCP : public Network
{
public:
	IOCP();
	virtual ~IOCP();

	inline bool IsInit() { return bInit; }
	operator bool() { return IsInit(); }
	bool IsInvalidHandle(HANDLE handle);

private:
	static const int MAX_WORKER = 8;

	void WorkerThread();

	std::vector< std::thread > vecThreads;/* = decltype(vecThreads)(MAX_WORKER);*/
	HANDLE hIOCP;

	std::atomic_bool bRunning = false;
	bool bInit = false;
};