#include <cassert>

#include "BaseGameEntity.h"

decltype(BaseGameEntity::m_iNextValidID) BaseGameEntity::m_iNextValidID = 0;

BaseGameEntity::BaseGameEntity(int id)
{
	SetID(id);
}

void BaseGameEntity::SetID(int val)
{
	//make sure the val is equal to or greater than the next available ID
	assert( (val >= m_iNextValidID) && "<BaseGameEntity::SetID>: invalid ID");

	m_ID = val;

	m_iNextValidID = m_ID+1;
}
