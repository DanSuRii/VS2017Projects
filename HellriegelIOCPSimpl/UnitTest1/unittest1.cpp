#include "stdafx.h"
#include "CppUnitTest.h"

#include "../HellriegelIOCPSimpl/utilities.h"
#include "../HellriegelIOCPSimpl/constants.h"
#include "../HellriegelIOCPSimpl/types.h"
#include "../HellriegelIOCPSimpl/Interfaces.h"
#include "../HellriegelIOCPSimpl/BufferPool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
			BufferPool bufPool;
			{
				auto cur = bufPool.GetNewBuf();
			}
		}

	};
}