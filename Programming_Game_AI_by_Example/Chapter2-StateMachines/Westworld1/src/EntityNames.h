#ifndef __ENTITYNAMES_H__
#define __ENTITYNAMES_H__

#include <string>

enum EENTITIES
{
	ENT_MINER_BOB,
	ENT_ELSA,

	ENT_CNT,
};

inline std::string GetNameOfEntity(int n)
{
	switch (n)
	{
	case ENT_MINER_BOB: return "Miner Bob";
	case ENT_ELSA: return "ELSA";

	default:
		return "UNKNOWN!";
	}
}

#endif // !__ENTITYNAMES_H__
