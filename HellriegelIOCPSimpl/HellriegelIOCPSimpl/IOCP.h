#pragma once
#include <thread>
#include <vector>
#include <algorithm>
#include <list>
#include <mutex>

#include "Network.h"

template< class _Managed >
class ContManaged
{
public:
	using _PManaged = std::shared_ptr<_Managed>;
	using TCONT_DATA = std::list< _PManaged >;
private:
	TCONT_DATA _contData;
	std::mutex mtxCont;
public:

	template< class T, class IterT = TCONT_DATA::iterator >
	std::shared_ptr<T> _New()
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
					{
						std::lock_guard< decltype(mtxCont)> grd(mtxCont);
						_cont.remove(_iterCur);
					}
					callOnce = false;
				}
			}

			IterT _iterCur;
			TCONT_DATA& _cont;
			std::atomic_bool callOnce = true;
		};

		IterT iter = _contData.end(); //it possible invalid, but do not care

		{
			std::lock_guard< decltype(mtxCont)> grd(mtxCont);
			iter = _contData.insert(_contData.end(), New<_Data>(_contData));
		}
		static_cast<_Data*>((*iter)->get())->_iterCur = iter;

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

private:
	
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
	ContManaged<IOCtx>			_contIOCtx;

	void Delete(ICompletionKey*);
	void Delete(IOCtx*);

	template<class T>
	void _NewCompKey()
	{

	}

	static const int MAX_WORKER = 8;

	void WorkerThread();

	std::vector< std::thread > vecThreads;/* = decltype(vecThreads)(MAX_WORKER);*/
	HANDLE hIOCP;

	bool bRunning = false;
	bool bInit = false;
};