#ifndef __MINERSTATES_H__
#define __MINERSTATES_H__

#include "state.h"

class Miner;

template< class Derived >
class CRTPSingleton_MinerState : public State< Miner >
{
public:
	static Derived* Instance()
	{
		static Derived _Instance;
		return &_Instance;
	}

protected:
	CRTPSingleton_MinerState()
	{
	}
};

class EnterMineAndDigForNugget : public CRTPSingleton_MinerState<EnterMineAndDigForNugget>
{
public:
private:
	// Inherited via CRTPSingleton_MinerState
	virtual void Enter(Miner *) override;
	virtual void Execute(Miner *) override;
	virtual void Exit(Miner *) override;
};

class VisitBankAndDepositGold : public CRTPSingleton_MinerState<VisitBankAndDepositGold>
{
public:
private:
	// Inherited via CRTPSingleton_MinerState
	virtual void Enter(Miner *) override;
	virtual void Execute(Miner *) override;
	virtual void Exit(Miner *) override;
};

class GoHomeAndSleepTilRested : public CRTPSingleton_MinerState<GoHomeAndSleepTilRested>
{
public:
private:
	// Inherited via CRTPSingleton_MinerState
	virtual void Enter(Miner *) override;
	virtual void Execute(Miner *) override;
	virtual void Exit(Miner *) override;
};

class QuenchThirst : public CRTPSingleton_MinerState<QuenchThirst>
{
public:
private:
	// Inherited via CRTPSingleton_MinerState
	virtual void Enter(Miner *) override;
	virtual void Execute(Miner *) override;
	virtual void Exit(Miner *) override;
};


#endif // !__MINERSTATES_H__
