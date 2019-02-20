#include "pch.h"
#include "ICompletionKey.h"

decltype(CKID::generator) CKID::generator = 0;

CKID::CKID()
	: _id( ++generator )
{

}
