#ifndef __MINER_H__
#define __MINER_H__

#include <memory>
#include <string>

#include "BaseGameEntity.h"
#include "StateMachine.h"

#include "locations.h"

class Miner : public BaseGameEntity
{
public:

	//the amount of gold a miner must have before he feels comfortable
	const int COMFORT_LEVEL = 5;
	//the amount of nuggets a miner can carry
	const int MAX_NUGGETS = 3;
	//above this value a miner is thirsty
	const int THIRST_LEVEL = 5;
	//above this value a miner is sleepy
	const int TIREDNESS_THRESHOLD = 5;

	//using MinerState = State< Miner >;
	using MinerStateMachine = StateMachine< Miner >;
	using PMinerStateMachine = std::shared_ptr< MinerStateMachine >;

	Miner(int id);

	void Update();

	auto GetFSM() const { return m_pStateMachine; }

	auto Location() const { return m_Location; }
	inline void ChangeLocation(location_type loc) { m_Location = loc; }

	void AddToGoldCarried(int val);
	void BuyAndDrinkAWhiskey() { m_iThirst = 0; m_iMoneyInBank -= 2; }
	
	void IncreaseFatigue() { m_iFatigue += 1; }
	void DecreaseFatigue() { m_iFatigue -= 1; }

	std::string GetName();

	bool PocketsFull() const { return m_iGoldCarried >= MAX_NUGGETS; }
	bool Thirsty() const;
	inline bool Fatigued() const { return m_iFatigue > TIREDNESS_THRESHOLD; };

	void Deposite();

	int Wealth() const { return m_iMoneyInBank; }
	bool IsWealthyEnough() { return Wealth() >= COMFORT_LEVEL; }

private:

	//an instance of the state machine class
	PMinerStateMachine m_pStateMachine;
	location_type m_Location;

	int m_iGoldCarried;
	int m_iMoneyInBank;
	int m_iThirst;
	int m_iFatigue;

	/*
	MinerState* m_pCurrentState;
	MinerState* m_pPreviousState;
	MinerState* m_pGlobalState;
	*/

};


#endif // !__MINER_H__
