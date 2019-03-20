#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include <cassert>
#include "state.h"

template< class entity_type >
class StateMachine
{
public:
	using ENTITY_STATE = State<entity_type>;

	StateMachine( entity_type* owner )
		: m_pOwner(owner)
		, m_pCurrentState(nullptr)
		, m_pPreviousState(nullptr)
		, m_pGlobalState(nullptr)
	{

	}

	//use these methods to initialize the FSM
	void SetCurrentState(ENTITY_STATE* s) { m_pCurrentState = s; }
	void SetGlobalState(ENTITY_STATE* s) { m_pGlobalState = s; }
	void SetPreviousState(ENTITY_STATE* s) { m_pPreviousState = s; }

	//call this to update the FSM
	void Update() const
	{
		//if a global state exists, call its execute method
		if (m_pGlobalState) m_pGlobalState->Execute(m_pOwner);

		//same for the current state
		if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}

	//change to a new state
	void ChangeState(ENTITY_STATE* pNewState)
	{
		assert(pNewState && "<StateMachine>: trying to change to a null state ");

		//keep a record of the previous state
		m_pPreviousState = m_pCurrentState;

		//call the exit method of the existing state
		m_pCurrentState->Exit(m_pOwner);

		//change state to the new state
		m_pCurrentState = pNewState;

		//call the entry method of the new state
		m_pCurrentState->Enter(m_pOwner);
	}

	//change state back to the previous state
	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}

	//accessors
	ENTITY_STATE* CurrentState() const { return m_pCurrentState; }
	ENTITY_STATE* GlobalState() const { return m_pGlobalState; }
	ENTITY_STATE* PreviousState() const { return m_pPreviousState; }

	//returns true if the current state's type is equal to the type of the
	//class passed as a parameter
	bool isInState(const ENTITY_STATE& st) const;

private:
	//a pointer to the agent that owns this instance
	entity_type* m_pOwner;

	ENTITY_STATE* m_pCurrentState;

	//a record of the last state the agent was in
	ENTITY_STATE* m_pPreviousState;

	//this state logic is called every time the FSM is updated
	ENTITY_STATE* m_pGlobalState;
};

#endif // !__STATEMACHINE_H__
