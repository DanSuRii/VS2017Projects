#include "pch.h"
#include "TestRepo.h"

#include <iostream>

TestRepo::TestRepo()
	: _cont{
		{ HELLOWORLD, std::make_shared< TestImpl<HELLOWORLD> >( "HELLOWORLD" ) },
		{ THHELLOWORLD, std::make_shared< TestImpl<THHELLOWORLD> >( "THHELLOWORLD" ) },
	}
{
}

TestRepo::~TestRepo()
{
}

#include <windows.h>
void TestRepo::Run()
{
	// TODO: Add your implementation code here.
	bool bExit = false;
	while (false == bExit)
	{
		for (auto cur : _cont)
		{
			auto& curInst = *(cur.second);
			std::cout <<  curInst.GetMyIdx() << ". " << curInst.GetName() << std::endl;
		}

		std::cout << "Select to Run (q to Exit): ";
		std::string strInput;
		std::cin >> strInput;
		if (strInput.empty())
		{
			std::cout << std::endl;
			continue;
		}

		switch (strInput.at(0))
		{
		case 'q': case 'Q':
			{
				bExit = true;
				continue;
			}
		}

		if (strInput.end() != std::find_if(strInput.begin(), strInput.end(), [](char& cur){ return cur < '0' || cur > '9'; }))
		{
			std::cout << "It is not a number" << std::endl;
			continue;
		}

		int select = std::stoi(strInput);
		auto iterSelected = _cont.find(select);
		if (iterSelected == _cont.end())
		{
			std::cout << "Selected does not exists" << std::endl;
			continue;
		}

		iterSelected->second->Do();
	}
}
