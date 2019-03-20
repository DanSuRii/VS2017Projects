#include "stdafx.h"
#include "Logger.h"

#include <iostream>

class ConsoleLogger : public ILogger
{
public:

	// Inherited via ILogger
	virtual void Log(std::string & strToWrite ) override
	{
		std::cout << strToWrite << std::endl;
	}
};

class FileLogger : public ILogger
{
public:

	// Inherited via ILogger
	virtual void Log(std::string &) override
	{
		//do nothing right now
	}

};

Logger::Logger()
	: _bRunning(true)
	, _contLogger{ std::make_shared< ConsoleLogger >(), std::make_shared< FileLogger >() }
{
	_worker = std::thread(&Logger::WorkerThread, this);
}

Logger::~Logger()
{
	{
		std::lock_guard<decltype(_mtxSync)> grd(_mtxSync);
		_bRunning = false;
	}
	_cv.notify_all();

	_worker.join();
}

void Logger::_PushLog(std::string && strToWrite)
{
	{
		std::lock_guard< decltype(_mtxCont) > grd(_mtxCont);
		_contStrToWrite.push(strToWrite);
	}

	_cv.notify_one();
}

void Logger::_PopAndWirte()
{
	std::string strToWrite;
	{
		std::lock_guard< decltype(_mtxCont) > grd(_mtxCont);
		if (_contStrToWrite.size())
		{
			strToWrite = _contStrToWrite.front();
			_contStrToWrite.pop();
		}
	}

	if (false == strToWrite.empty())
	{
		for (auto& cur : _contLogger)
		{
			cur->Log(strToWrite);
		}
	}
}

void Logger::WorkerThread()
{
	while (_bRunning)
	{
		{
			std::unique_lock<decltype(_mtxSync)> uq(_mtxSync);
			_cv.wait(uq);
		}

		while (_contStrToWrite.size())
		{
			_PopAndWirte();
			std::this_thread::yield();
		}
	}

	while (_contStrToWrite.size() > 0) _PopAndWirte();
}
