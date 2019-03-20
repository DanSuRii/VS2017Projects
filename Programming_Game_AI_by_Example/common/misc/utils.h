#ifndef __UTILS_H__
#define __UTILS_H__

#include <cassert>
#include <cmath>
#include <numeric>
#include <limits>

#include <random>

namespace NS_DP
{
	inline bool isEqual(double a, double b)
	{
		/*
		bool bEqual(false);
		if (std::fabs(a - b) < std::numeric_limits<double>::epsilon())
		{

		}
		*/
		return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
	}
	//returns a random integer between x and y
	inline int RandInt(int min, int max)
	{
		assert( max >= min && "<RandInt>: max is less than x" );
		return rand() % (max - min + 1) + min;
	}

	//returns a random double between zero and 1
	inline double RandFloat() { return std::rand() / (RAND_MAX + 1.0); }
}

#endif // !__UTILS_H__

