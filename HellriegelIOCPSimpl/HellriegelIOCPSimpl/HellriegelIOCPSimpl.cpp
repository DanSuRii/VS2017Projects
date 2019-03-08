// HellriegelIOCPSimpl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "IOCP.h"

#include "BufferPool.h"

int main()
{
	//Make some code for genereate debug info....lol

	BufferPool bufferPool; //it provides safe to every objects return , it will guarentee bufferPool dispose after IOCP
	{
		auto cur = bufferPool.GetNewBuf();

		PIBufHandle newHandle(cur);
	}

	IOCP iocp(bufferPool);
	if (false == iocp.IsInit())
	{
		LOG_FN("iocp init failed");
		return 0;
	}

	if (false == iocp.Listen("38000"))
	{
		LOG_FN("Listen Failed");
		return 0;
	}

	Log("Alive unit any key pressed..");
	::system("pause");

	//TODO: guarentee Buffer pool returned
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

