#include "pch.h"
#include "BufferPool.h"

BufferPool::BufferPool(size_t initLen)
	:_listAvailable(initLen)
	, currentPoolSize(initLen)
{

}

BufferPool::BufferPool()
	: BufferPool(DEFAULT_POOL_SIZE)
{
}

PIBufHandle BufferPool::GetNewBuf()
{
	T_ELEM toRet(nullptr);
	{
		std::unique_lock< decltype(mtxAvail) > uqgrd(mtxAvail);
		if (_listAvailable.size() <= 0)
		{
			_growAvailable();
		}

		toRet = _listAvailable.front();
		_listAvailable.pop_front();
	}

	TIter iter;
	{
		std::unique_lock< decltype(mtxUse) > uqgrd(mtxUse);
		iter = _listInUse.insert(_listInUse.end(), toRet);
	}

	return New<BUF_HANDLE>(iter, std::bind(&BufferPool::_Recycle, this, std::placeholders::_1));
}

void BufferPool::_Recycle(TIter iter)
{
	{
		std::unique_lock< decltype(mtxAvail) > uqgrd(mtxAvail);
		_listAvailable.push_back(*iter);
	}

	{
		std::unique_lock< decltype(mtxUse) > uqgrd(mtxUse);
		_listInUse.erase(iter);
	}
}

void BufferPool::_growAvailable()
{
	T_CONT _listTemp(currentPoolSize);
	_listAvailable.splice(_listAvailable.end(), std::move(_listTemp));

	currentPoolSize *= 2;
}
