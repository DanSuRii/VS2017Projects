#ifndef __MINERSWIFE_H__
#define __MINERSWIFE_H__

#include "locations.h"
#include "StateMachine.h"
#include "BaseGameEntity.h"

#include <memory>


class MinersWife : public BaseGameEntity
{
public:
	using MWifeStateMachine = StateMachine< MinersWife >;
	using PMinerStateMachine = std::shared_ptr< MWifeStateMachine >;

	MinersWife(int id);

	auto GetFSM() { return m_pStateMachine; }
	void Update();

private:

	location_type		m_Location;

	PMinerStateMachine	m_pStateMachine;

};

#endif