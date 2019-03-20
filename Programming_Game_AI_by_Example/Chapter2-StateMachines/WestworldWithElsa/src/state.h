#ifndef __STATE_H__
#define __STATE_H__

template< class entity_type >
class State
{
public:
	using TENTITY = entity_type;

	virtual void Enter(TENTITY*) = 0;
	virtual void Execute(TENTITY*) = 0;
	virtual void Exit(TENTITY*) = 0;

	virtual ~State()
	{

	}

private:
};



#endif //__STATE_H__