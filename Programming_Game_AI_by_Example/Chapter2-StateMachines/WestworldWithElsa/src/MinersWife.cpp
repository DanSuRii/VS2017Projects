#include "MinerWifeOwnedStates.h"

#include "MinersWife.h"

MinersWife::MinersWife(int id)
	: m_pStateMachine(std::make_shared<MWifeStateMachine>(this))
	, m_Location(shack)
	, BaseGameEntity(id)
{
	m_pStateMachine->SetCurrentState(DoHouseWork::Instance());

	m_pStateMachine->SetGlobalState(WifesGlobalState::Instance());
}

void MinersWife::Update()
{
	m_pStateMachine->Update();
}
