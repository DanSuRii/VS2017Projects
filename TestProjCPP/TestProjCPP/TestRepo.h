#pragma once

#include "Test.h"

class TestRepo
{
public:
	TestRepo();
	virtual ~TestRepo();

private:

	std::map<int ,PITest> _cont;
public:
	void Run();
};

