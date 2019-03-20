#include "MinerStates.h"

#include "entitynames.h"
#include "miner.h"

#include "misc/Logger.h"
#include "misc/utils.h"

void EnterMineAndDigForNugget::Enter(Miner * pMiner)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (pMiner->Location() != goldmine)
	{
		LOG(GetNameOfEntity::Get(pMiner->ID()), ": ", "Walkin' to the goldmine");

		pMiner->ChangeLocation(goldmine);
	}

}

void EnterMineAndDigForNugget::Execute(Miner * pMiner)
{
	//if the miner is at the goldmine he digs for gold until he
	//is carrying is excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pMiner->AddToGoldCarried(1);

	pMiner->IncreaseFatigue();

	LOG(pMiner->GetName(), ": Pickin'up a nugget");

	//if enough gold mined, go and put it in the bank
	if (pMiner->PocketsFull())
	{
		pMiner->GetFSM()->ChangeState( VisitBankAndDepositGold::Instance() );
	}

	if (pMiner->Thirsty())
	{
		pMiner->GetFSM()->ChangeState( QuenchThirst::Instance() );
	}
}

void EnterMineAndDigForNugget::Exit(Miner * pMiner)
{
	LOG( pMiner->GetName(), ": Ah'm leavin' the goldmine with mah pockets full o' sweet gold" );
}

void VisitBankAndDepositGold::Enter(Miner * pMiner)
{
	//on entry the miner makes sure he is located at the bank
	if (pMiner->Location() != bank)
	{
		LOG( pMiner->GetName(), ": Goin' to the bank. Yes siree" );
		
		pMiner->ChangeLocation(bank);
	}
}

void VisitBankAndDepositGold::Execute(Miner * pMiner)
{
	//deposit the gold
	pMiner->Deposite();

	LOG( pMiner->GetName(), ": Depositing gold. Total savings now: ", pMiner->Wealth() );

	//wealthy enough to have a well earned rest?
	if (pMiner->IsWealthyEnough())
	{
		LOG( pMiner->GetName(), ": WooHoo! Rich enough for now. Back home to mah li'lle lady" );
		pMiner->GetFSM()->ChangeState( GoHomeAndSleepTilRested::Instance() );
	}
	else
	{ //otherwise get more gold
		pMiner->GetFSM()->ChangeState( EnterMineAndDigForNugget::Instance() );
	}
}

void VisitBankAndDepositGold::Exit(Miner * pMiner)
{
	LOG( pMiner->GetName(), ": Leavin' the bank" );
}

void GoHomeAndSleepTilRested::Enter(Miner * pMiner)
{
	if (pMiner->Location() != shack)
	{
		LOG(pMiner->GetName(), ": Walkin' home");

		pMiner->ChangeLocation(shack);
	}
}

void GoHomeAndSleepTilRested::Execute(Miner * pMiner)
{
	if (false == pMiner->Fatigued())
	{
		LOG( pMiner->GetName(), ": What a god darn fantastic nap! Time to find more gold" );

		pMiner->GetFSM()->ChangeState( EnterMineAndDigForNugget::Instance() );
	}
	else
	{
		//sleep
		pMiner->DecreaseFatigue();

		LOG( pMiner->GetName(), ": ZzzZzz..." );
	}
}

void GoHomeAndSleepTilRested::Exit(Miner * pMiner )
{
	LOG( pMiner->GetName(), ": Leaving the house" );
}

void QuenchThirst::Enter(Miner * pMiner)
{
	if (pMiner->Location() != saloon)
	{
		pMiner->ChangeLocation(saloon);
		
		LOG( pMiner->GetName(), ": Boy, ah sure is thusty! Walking the saloon" );
	}
}

void QuenchThirst::Execute(Miner * pMiner)
{
	if (pMiner->Thirsty())
	{
		pMiner->BuyAndDrinkAWhiskey();

		LOG( pMiner->GetName(), ": That's mighty fine sippin' liquer" );

		pMiner->GetFSM()->ChangeState( EnterMineAndDigForNugget::Instance() );
	}
	else
	{
		for (int nRepeat = 3; nRepeat; --nRepeat)
		{
			LOG( "Error!" );
		}
	}
}

void QuenchThirst::Exit(Miner * pMiner)
{
	LOG( pMiner->GetName(), ": Leaving the saloon, feelin' good" );
}
