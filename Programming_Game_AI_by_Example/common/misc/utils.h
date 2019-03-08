#ifndef __UTILS_H__
#define __UTILS_H__

#include <cmath>
#include <numeric>
#include <limits>

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
}

#endif // !__UTILS_H__

