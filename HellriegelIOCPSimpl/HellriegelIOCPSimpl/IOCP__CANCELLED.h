#pragma once
#include <thread>
#include <vector>
#include <algorithm>
#include <list>

#include "Network.h"

template<class T>
void MsgHandler(T& rhs)
{
}

class IMsgHandler
{

};

template< class T >
class MsgHandler : public IMsgHandler
{
public:
	virtual void DoHandle(T&) = 0;
};

class IMsgHandlerFactory
{
public:
};

class AbstractVisitor
{
public:
	template<class T>
	void Dispatch(T& rhs)
	{
		MsgHandler<T>* handler = static_cast<MsgHandler<T>*>GetMsgHandler();

		handler.DoHandle(rhs);
	}

	virtual IMsgHandler* GetMsgHandler() = 0;

protected:
	IMsgHandler*	pMyHandler;
};

class IOCtx
{
public:
	virtual void Accept(AbstractVisitor&) = 0;
};

class IORead : public IOCtx
{
public:
	virtual void Accept(AbstractVisitor& visitor)
	{
		// *this
		visitor.Dispatch(*this);
	};
};


class IOWrite : public IOCtx
{
public:
	virtual void Accept(AbstractVisitor& visitor)
	{
		// *this
		visitor.Dispatch(*this);
	};

};

class IOAccept : public IOCtx
{
public:
	virtual void Accept(AbstractVisitor& visitor)
	{
		// *this
		visitor.Dispatch(*this);
	};

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

	class WriteHandle : public MsgHandler<IOWrite>
	{
		// Inherited via MsgHandler
		virtual void DoHandle(IOWrite &) override;
	};

	class ConcreteVisitor : public AbstractVisitor
	{
	public:
		// Inherited via AbstractVisitor
		virtual IMsgHandler * GetMsgHandler() override
		{
			return new WriteHandle();
		}
	
	};
	
	using PICompletionKey = std::shared_ptr<ICompletionKey>;
	using TCONT_DATA = std::list< PICompletionKey >;
	
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
	
	std::list< PICompletionKey > _contClt;

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

		auto iter = _contClt.insert(_contClt.end(), New<_Data>( _contClt ));
		static_cast<_Data*>((*iter)->get())->_iterCur = iter;

		return *iter;
	}
	
	void _DeleteClt(ICompletionKey* pClt)
	{
		if (nullptr == pClt)
			return;

		pClt->Dispose();
	}
	

	static const int MAX_WORKER = 8;

	void WorkerThread();

	std::vector< std::thread > vecThreads;/* = decltype(vecThreads)(MAX_WORKER);*/
	HANDLE hIOCP;

	bool bRunning = false;
	bool bInit = false;
};