#pragma once
#include <thread>
#include <vector>
#include <algorithm>
#include <list>
#include <mutex>
#include <functional>

#include "Network.h"

template< class _Managed >
class ContManaged
{
public:
	using _PManaged = std::shared_ptr<_Managed>;
	using TCONT_DATA = std::list< _PManaged >;
	using IterT = typename TCONT_DATA::iterator;
	using _FN_DISPOSE = std::function< void(IterT) >;
private:
	TCONT_DATA _contData;
	std::mutex _mtxCont;
	
public:

	template< class T, class ... Args >
	auto _New(Args&& ... args )
	{
		struct _Data : public T
		{
			_Data(_FN_DISPOSE fnDispose, TCONT_DATA& cont, Args&& ...args)
				: _fnDispose(fnDispose), T( std::forward<Args>(args)...)
			{

			}

			~_Data()
			{
			}

			virtual void Dispose() override
			{
				if (callOnce)
				{
					if (_fnDispose) _fnDispose(_iterCur);
					callOnce = false;
				}
			}

			IterT _iterCur;
			//TCONT_DATA& _cont;
			_FN_DISPOSE _fnDispose;

			std::atomic_bool callOnce = true;
		};

		IterT iter = _contData.end(); //it possible invalid, but do not care

		auto fnDispose = [&](IterT iterCur) {
			{
				std::lock_guard< decltype(_mtxCont)> grd(_mtxCont);
				_contData.erase(iterCur);
			}
		};		

		{
			std::lock_guard< decltype(_mtxCont)> grd(_mtxCont);
			iter = _contData.insert(_contData.end(), New<_Data>(fnDispose, _contData, std::forward<Args>(args)...));
		}
		auto ptrCur = (*iter);
		static_cast<_Data*>( ptrCur.get() )->_iterCur = iter;

		return *iter;
	}

	void _Delete(_Managed* pManaged)
	{
		if (nullptr == pManaged)
			return;

		pManaged->Dispose();
	}
};

class IOCP : public Network
{
public:
	IOCP();
	virtual ~IOCP();

	inline bool IsInit() { return bInit; }
	operator bool() { return IsInit(); }
	bool IsInvalidHandle(HANDLE handle);

	//is it possible to service by ports?
	bool Listen( std::string strPort );

private:

	bool ReservAccept( class Listener& listener );
	
	/*
	struct Data : public ICompletionKey
	{
		~Data()
		{

		}
		TCONT_DATA::iterator iterCur;
	};
	using PData = std::shared_ptr<Data>;
	*/
#if false
	using PICompletionKey = std::shared_ptr<ICompletionKey>;
	using TCONT_DATA = std::list< PICompletionKey >;

	TCONT_DATA _contClt;

	void _AllocNewClt();

	template< class T >
	std::shared_ptr<T> _NewClt()
	{
		struct _Data : public T
		{
			_Data(TCONT_DATA& cont)
				: _cont(cont)
			{

			}

			~_Data()
			{
			}

			virtual void Dispose() override
			{
				if (callOnce)
				{
					_cont.remove(_iterCur);
					callOnce = false;
				}
			}

			TCONT_DATA::iterator _iterCur;
			TCONT_DATA& _cont;
			std::atomic_bool callOnce = true;
		};

		auto iter = _contClt.insert(_contClt.end(), New<_Data>(_contClt));
		static_cast<_Data*>((*iter)->get())->_iterCur = iter;

		return *iter;
	}

	void _DeleteClt(ICompletionKey* pClt)
	{
		if (nullptr == pClt)
			return;

		pClt->Dispose();
	}

#endif // false	
	
	ContManaged<ICompletionKey> _contCompl;
	template<typename T, class ... Args>
	PICompletionKey NewKey(Args&&... args)
	{
		return _contCompl._New<T>(std::forward<Args>(args)...);
	}

	ContManaged<IOCtx>			_contIOCtx;
	template<typename T, class ... Args>
	PIOCtx NewIO( Args&&... args )
	{
		return _contIOCtx._New<T>( std::forward<Args>(args) ... );
	}

	void Delete(ICompletionKey*);
	void Delete(IOCtx*);


	static const int MAX_WORKER = 8;

	void WorkerThread();

	std::vector< std::thread > vecThreads;/* = decltype(vecThreads)(MAX_WORKER);*/
	HANDLE hIOCP;

	bool bRunning = false;
	bool bInit = false;
};