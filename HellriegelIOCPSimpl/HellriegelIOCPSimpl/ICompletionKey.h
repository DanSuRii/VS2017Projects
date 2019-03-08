#pragma once

#include <memory>

class CKID
{
public:
	CKID();

	const auto GetID() { return _id; };
private:
	const std::int64_t _id;

	static std::atomic_int64_t generator;
};


class ICompletionKey
{
public:
	ICompletionKey(HANDLE hIocp, SOCKET sockToJoin);
	/*
	ICompletionKey(ClientID idClt)
		: _idClt(idClt)
	{
	};
	*/

	virtual ~ICompletionKey()
	{

	}

	ICompletionKey(const ICompletionKey&) = delete;
	ICompletionKey& operator=(const ICompletionKey&) = delete;

	virtual void Dispose() = 0;
	//virtual void Handle(class IOCtx*) = 0;

	inline auto GetID() { return idCur.GetID(); }

	//TODO:must check all derived chain
	bool IsInit() { return bInit; }
private:
	HANDLE _hIocp;

	CKID idCur;

	std::atomic_bool bInit = false;
};
using PICompletionKey = std::shared_ptr<ICompletionKey>;