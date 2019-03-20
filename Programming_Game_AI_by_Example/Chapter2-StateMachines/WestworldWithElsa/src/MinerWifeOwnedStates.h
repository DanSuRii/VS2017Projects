#ifndef __MINERWIFEOWNEDSTATES_H__
#define __MINERWIFEOWNEDSTATES_H__

#include "state.h"

class MinersWife;

using MinersWifeState = State< MinersWife >;

template< class Derived >
class CRTPSingleton_MinerState : public MinersWifeState
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


class WifesGlobalState : public CRTPSingleton_MinerState<WifesGlobalState>
{
public:

	WifesGlobalState(){}	

private:

	// Inherited via CRTPSingleton_MinerState
	virtual void Enter(MinersWife *) override;
	virtual void Execute(MinersWife *) override;
	virtual void Exit(MinersWife *) override;
};


class DoHouseWork : public CRTPSingleton_MinerState<DoHouseWork>
{
public:
	DoHouseWork(){}
private:

	// Inherited via CRTPSingleton_MinerState
	virtual void Enter(MinersWife *) override;
	virtual void Execute(MinersWife *) override;
	virtual void Exit(MinersWife *) override;
};

class VisitBathroom : public CRTPSingleton_MinerState<VisitBathroom>
{
public:

private:

	// Inherited via CRTPSingleton_MinerState
	virtual void Enter(MinersWife *) override;
	virtual void Execute(MinersWife *) override;
	virtual void Exit(MinersWife *) override;
};

#endif // !__MINERWIFEOWNEDSTATES_H__
