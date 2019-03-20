#ifndef __STATE_H__
#define __STATE_H__

class Miner;

class State
{
public:
	//virtual void Execute( Miner* ) = 0;
	virtual ~State()
	{

	}

	//this will execute when the state is entered
	virtual void Enter(Miner*) = 0;

	//this is called by the miner's update function each update step
	virtual void Execute(Miner*) = 0;

	//this will execute when the state is exited
	virtual void Exit(Miner*) = 0;
	

private:
};

#endif // !__STATE_H__
