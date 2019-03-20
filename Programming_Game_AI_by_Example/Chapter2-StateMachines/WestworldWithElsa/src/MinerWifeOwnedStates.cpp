#include <functional>

#include "entitynames.h"

#include "misc/utils.h"
#include "misc/Logger.h"
#include "MinerWifeOwnedStates.h"

#include "MinersWife.h"

void WifesGlobalState::Enter(MinersWife *)
{
}

void WifesGlobalState::Execute(MinersWife * wife)
{
	//1 in 10 chance of needing the bathroom
	if (NS_DP::RandFloat() < 0.1)
	{
		wife->GetFSM()->ChangeState(VisitBathroom::Instance());
	}
}

void WifesGlobalState::Exit(MinersWife *)
{
}

void DoHouseWork::Enter(MinersWife *)
{
}

void DoHouseWork::Execute(MinersWife * wife)
{
	const char* const workList[]
	{
		": Moppin' the floor",
		": Washin' the dishes",
		": Makin' the bed",
	};

	static std::function< void(void) > fns[]
	{
		[&] {},
		[&] {},
		[&] {},
	};

	int act = NS_DP::RandInt(0, 2);
	if (act < 0 || act >= _countof(workList))
	{
		LOG_FILELINE_FN("Failed to execute. act out of range between 0 and ", _countof(workList));
		return;
	}

	LOG( GetNameOfEntity::Get( wife->ID() ), workList[act] );
}

void DoHouseWork::Exit(MinersWife *)
{
}

void VisitBathroom::Enter(MinersWife * wife)
{
	LOG( GetNameOfEntity::Get(  wife->ID() ), ": Walkin' to the can. Need to powda mah pretty li'lle nose" );
}

void VisitBathroom::Execute(MinersWife * wife)
{
	LOG( GetNameOfEntity::Get( wife->ID() ), ": Ahhhhh! Sweet relief" );

	wife->GetFSM()->RevertToPreviousState();
}

void VisitBathroom::Exit(MinersWife * wife)
{
	LOG( GetNameOfEntity::Get( wife->ID() ), ": Leavin' the Jon" );
}
