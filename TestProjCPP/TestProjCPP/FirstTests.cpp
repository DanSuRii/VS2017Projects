#include "pch.h"

#include "Test.h"

#include <iostream>

#include <thread>

template<>
void TestImpl< HELLOWORLD >::Do()
{
	std::cout << "Hello! World" << std::endl;
}

template<>
void TestImpl<THHELLOWORLD>::Do()
{	
	std::thread thToHello([] {
		
			std::cout << "Hello World From Thread: " << std::this_thread::get_id() << std::endl;
	});
	
	thToHello.join();	
}
