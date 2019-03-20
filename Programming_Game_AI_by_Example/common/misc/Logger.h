#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <memory>
#include <list>
#include <string>
#include <thread>
#include <queue>

#include <mutex>
#include <condition_variable>
#include <thread>

#include <atomic>

#include <sstream>

class ILogger
{
public:
	virtual void Log(std::string&) = 0;
};
using PILogger = std::shared_ptr< ILogger >;

class Logger
{
	Logger();
public:
	using _ContTy = std::list<PILogger>;

	virtual ~Logger();

	template< class ...Args >
	void Log( Args&& ... args )
	{
		std::stringstream strToWrite;
		int a[]{ (strToWrite << args ,0) ... };
		static_cast<void>(a);

		strToWrite << '\n';

		_PushLog(strToWrite.str());
	}

	static inline Logger& GetInstance()
	{
		static Logger _Instance;
		return _Instance;
	}
	
private:

	void WorkerThread();
	
	void _PushLog(std::string&&);
	void _PopAndWirte();


	std::condition_variable			_cv;
	std::mutex						_mtxSync;	

	std::thread						_worker;

	std::mutex						_mtxCont;
	std::queue< std::string >		_contStrToWrite;

	std::atomic_bool				_bRunning;

	_ContTy _contLogger;
};

#define LOG( ... ) Logger::GetInstance().Log( __VA_ARGS__ )
#define LOG_FN( ... ) LOG( __FUNCSIG__, ">>" , __VA_ARGS__ )
#define LOG_FILELINE_FN( ... ) LOG( __FILE__, "(", __LINE__ ,"):", __FUNCSIG__, ">> " , __VA_ARGS__ )

#endif // !__LOGGER_H__
