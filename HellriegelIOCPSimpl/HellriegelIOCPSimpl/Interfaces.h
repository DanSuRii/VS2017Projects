#pragma once

#include <memory>

class IBufHandle
{
public:
	virtual char* GetBufPtr() = 0;
	virtual size_t GetBufferSize() = 0;
};
using PIBufHandle = std::shared_ptr<IBufHandle>;

class IBufferPool
{
public:
	virtual PIBufHandle GetNewBuf() = 0;
};

