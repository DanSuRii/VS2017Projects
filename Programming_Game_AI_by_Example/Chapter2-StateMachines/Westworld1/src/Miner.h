#ifndef __MINER_H__
#define __MINER_H__

#include "State.h"
#include "BaseGameEntity.h"

enum location_type
{
	goldmine,
	bank,
	saloon,
	shack,
	ELOC_CNT,
};

//the amount of nuggets a miner can carry
const int MAX_NUGGETS = 3;

//above this value a miner is thirsty
const int ThirstLevel = 5;

//the amount of gold a miner must have before he feels comfortable
const int COMFORT_LEVEL = 5;

//above this value a miner is sleepy
const int TIREDNESS_THRESHOLD = 5;

class Miner : public BaseGameEntity
{
public:
	Miner(int ID);

	//this must be implemented
	virtual void Update() override;

	//this method changes the current state to the new state
	void ChangeState(State* pNewState);

	location_type Location() { return m_Location; }

	void ChangeLocation(location_type loc);

	void AddToGoldCarried(const int val);

	inline void IncreaseFatigue() { m_iFatigue += 1; }
	inline void DecreaseFatigue() { m_iFatigue -= 1; }

	inline bool PocketsFull() const { return m_iGoldCarried >= MAX_NUGGETS; }
	inline bool Thirsty() const { return m_iThirst >= ThirstLevel; }

	void DepositeToBank();
	
	int Wealth() const { return m_iMoneyInBank; }
	inline bool IsComport() { return Wealth() >= COMFORT_LEVEL; };

	bool Fatigued() const;

	void BuyAndDrinkAWhiskey() { m_iThirst = 0; m_iMoneyInBank -= 2; }

private:
	void _AddToWealth(const int val);
	inline void _SetGoldCarried(const int val) { m_iGoldCarried = val; }
	inline auto _GetGoldCarried() { return m_iGoldCarried; }



	//a pointer to an instance of a State
	State*	m_pCurrentState;

	// the place where the miner is currently situated
	location_type	m_Location;

	//how many nuggets the miner has in his pockets
	int m_iGoldCarried;

	//how much money the miner has deposited in the bank
	int m_iMoneyInBank;

	//the higher the value, the thirstier the miner
	int m_iThirst;

	//the igher the value, the more tired the miner
	int m_iFatigue;
};

#endif // !__MINER_H__
