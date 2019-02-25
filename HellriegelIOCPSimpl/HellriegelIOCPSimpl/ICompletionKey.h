#pragma once

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
	ICompletionKey()
	{
		_idClt = -1;
	}
	ICompletionKey(ClientID idClt)
		: _idClt(idClt)
	{
	};

	virtual ~ICompletionKey()
	{

	}

	ICompletionKey(const ICompletionKey&) = delete;
	ICompletionKey& operator=(const ICompletionKey&) = delete;

	virtual void Dispose() = 0;
	virtual void Handle(class IOCtx*) = 0;

	inline ClientID GetID() { _idClt; }
private:
	ClientID _idClt;
};
