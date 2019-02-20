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

private:

};
