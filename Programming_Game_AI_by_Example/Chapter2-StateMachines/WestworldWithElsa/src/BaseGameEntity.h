#ifndef __BASEGAMEENTITY_H__
#define __BASEGAMEENTITY_H__

#include <atomic>

class BaseGameEntity
{
public:
	BaseGameEntity(int id);

	auto ID() const { return m_ID; }
private:

	//this must be called within the constructor to make sure the id is set
	//correctly. It verifies that the value passed to the method is greater
	//or equal to the next valid id, before setting the id and incrementing
	//the next valid id
	void SetID(int val);

	//every entity must have a unique identifying number
	int m_ID;

	//this is the next valid ID. Each time a BaseGameEntity is instantiated
	//this value is updated
	static std::atomic_int32_t m_iNextValidID;
};

#endif // !__BASEGAMEENTITY_H__
