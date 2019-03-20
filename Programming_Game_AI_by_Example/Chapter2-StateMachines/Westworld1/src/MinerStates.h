#ifndef __MINERSTATES_H__
#define __MINERSTATES_H__

#include "State.h"

template< class Derived >
class TStateSingleton : public State
{
protected:
	TStateSingleton() {}
public:
	static Derived* Instance()
	{
		static Derived _Instance;
		return &_Instance;
	}
};

class EnterMineAndDigForNugget : public TStateSingleton<EnterMineAndDigForNugget>
{
public:


private:

	// Inherited via State
	virtual void Enter(Miner *) override;

	virtual void Execute(Miner *) override;

	virtual void Exit(Miner *) override;

};

class VisitBankAndDepositGold : public TStateSingleton<VisitBankAndDepositGold>
{
public:

private:
	// Inherited via State
	virtual void Enter(Miner *) override;
	virtual void Execute(Miner *) override;
	virtual void Exit(Miner *) override;
};

class GoHomeAndSleepTilRested : public TStateSingleton<GoHomeAndSleepTilRested>
{
public:
private:

	// Inherited via TStateSingleton
	virtual void Enter(Miner *) override;
	virtual void Execute(Miner *) override;
	virtual void Exit(Miner *) override;
};

class QuenchThirst : public TStateSingleton<QuenchThirst>
{
public:

private:
	// Inherited via State
	virtual void Enter(Miner *) override;
	virtual void Execute(Miner *) override;
	virtual void Exit(Miner *) override;
};

#endif // !__MINERSTATES_H__
