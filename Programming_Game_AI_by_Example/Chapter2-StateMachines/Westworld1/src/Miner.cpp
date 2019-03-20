#include <cassert>

#include "Miner.h"

#include "MinerStates.h"

Miner::Miner(int ID)
	: BaseGameEntity(ID)
	, m_Location(shack)
	, m_iGoldCarried(0)
	, m_iMoneyInBank(0)
	, m_iThirst(0)
	, m_iFatigue(0)
	, m_pCurrentState(GoHomeAndSleepTilRested::Instance())
{
}

void Miner::Update()
{
	m_iThirst += 1;

	if (m_pCurrentState)
		m_pCurrentState->Execute(this);
}

void Miner::ChangeState(State * pNewState)
{
	//make sure both states are valid before attempting to
	//call their methods
	assert( m_pCurrentState && pNewState );

	//call the exit method of the existing state
	m_pCurrentState->Exit(this);

	//change state to the new state;
	m_pCurrentState = pNewState;

	//call the entry method of the new state
	m_pCurrentState->Enter(this);
}

void Miner::ChangeLocation(location_type loc)
{
	m_Location = loc;
}

void Miner::AddToGoldCarried(const int val)
{
	m_iGoldCarried += val;

	if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Miner::DepositeToBank()
{
	_AddToWealth( _GetGoldCarried() );
	_SetGoldCarried(0);
}

bool Miner::Fatigued() const
{
	return m_iFatigue > TIREDNESS_THRESHOLD;
}

void Miner::_AddToWealth(const int val)
{
	m_iMoneyInBank += val;

	if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

