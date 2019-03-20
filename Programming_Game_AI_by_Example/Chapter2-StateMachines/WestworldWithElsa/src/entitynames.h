#ifndef __ENTITYNAMES_H__
#define __ENTITYNAMES_H__

#include <string>

enum
{
	ENT_MINER_BOB,

	ENT_ELSA,

	ENT_CNT,
};

class GetNameOfEntity
{
	GetNameOfEntity()
	{
		nameOfEntities[ENT_MINER_BOB] = "Miner Bob";
		nameOfEntities[ENT_ELSA] = "Elsa";
	}
	GetNameOfEntity(const GetNameOfEntity&) = delete;
	GetNameOfEntity& operator=(const GetNameOfEntity&) = delete;
public:

	static std::string Get(int n)
	{
		static GetNameOfEntity _Instance;

		if (n < 0 || n >= ENT_CNT)
			return "UNKNOWN!";

		return _Instance.nameOfEntities[n];
	}

private:
	std::string nameOfEntities[ENT_CNT];
};

#endif // !__ENTITYNAMES_H__