#pragma once

#include <memory>
#include <list>
#include <shared_mutex>

class IBufHandle
{
public:
	virtual char* GetBufPtr() = 0;
	virtual size_t GetBufferSize() = 0;
};
using PIBufHandle = std::shared_ptr<IBufHandle>;

class BufferPool
{
public:
	static const int DEFAULT_POOL_SIZE = 256;
	BufferPool(size_t initLen);

	BufferPool();
	struct Buffer
	{
		BYTEBUFFER buf;
	};

	using T_ELEM = std::shared_ptr<Buffer>;
	using T_CONT = std::list< T_ELEM >;
	using TIter = typename T_CONT::iterator;


	PIBufHandle GetNewBuf();

private:
	void _Recycle(TIter iter);

	struct BUF_HANDLE : public IBufHandle
	{
		using FN_DESTRUCT = std::function< void(TIter) >;

		BUF_HANDLE(TIter iter, FN_DESTRUCT fn) :iterInUse(iter), _fnDestruct(fn) {};
		BUF_HANDLE(const BUF_HANDLE&) = delete;
		BUF_HANDLE operator=(const BUF_HANDLE&) = delete;
		BUF_HANDLE(BUF_HANDLE&& rhs) = default;

		~BUF_HANDLE()
		{
			if (_fnDestruct)
				_fnDestruct(iterInUse);
		}

		virtual char*	GetBufPtr() override { return (*iterInUse)->buf; }
		virtual size_t	GetBufferSize() override { return sizeof((*iterInUse)->buf); }

	private:
		FN_DESTRUCT _fnDestruct;
		TIter iterInUse;
	};

	void _growAvailable();

	std::shared_mutex mtxUse;
	T_CONT _listInUse;

	std::shared_mutex mtxAvail;
	T_CONT _listAvailable;

	size_t currentPoolSize = 0;

};