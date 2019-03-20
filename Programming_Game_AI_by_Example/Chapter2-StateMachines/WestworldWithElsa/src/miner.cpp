#include "miner.h"
#include "MinerStates.h"

#include "entitynames.h"

#include "misc/Logger.h"

Miner::Miner(int id)
	: m_Location(shack)
	, m_iGoldCarried(0)
	, m_iMoneyInBank(0)
	, m_iThirst(0)
	, m_iFatigue(0)
	, BaseGameEntity(id)
{
	//set up state machine
	m_pStateMachine = std::make_shared< MinerStateMachine >(this);

	m_pStateMachine->SetCurrentState(GoHomeAndSleepTilRested::Instance());
	//m_pStateMachine->SetGlobalState(  )
}

void Miner::Update()
{
	++m_iThirst;
	m_pStateMachine->Update();
}

void Miner::AddToGoldCarried(int val)
{
	m_iGoldCarried += val;
	if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

std::string Miner::GetName()
{
	return GetNameOfEntity::Get(ID());
}

bool Miner::Thirsty() const
{
	return m_iThirst >= THIRST_LEVEL;
}

void Miner::Deposite()
{
	m_iMoneyInBank += std::move(m_iGoldCarried);
	if (m_iGoldCarried != 0)
	{
		LOG( "!!!WARNING!!! gold in carried does not moved out" );
		m_iGoldCarried = 0;
	}
}
