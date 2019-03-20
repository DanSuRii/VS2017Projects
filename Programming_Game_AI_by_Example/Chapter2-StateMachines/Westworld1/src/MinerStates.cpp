#include "common.h"

#include "MinerStates.h"

#include "Miner.h"
#include "EntityNames.h"


void EnterMineAndDigForNugget::Enter(Miner * pMiner)
{
	//if the miner is not already located at the gold mine, he must
	//change location to the gold mine

	if (pMiner->Location() != goldmine)
	{		
		LOG( GetNameOfEntity(pMiner->ID()) , ": ", "Walkin' to the gold mine" );

		pMiner->ChangeLocation(goldmine);
	}
}

void EnterMineAndDigForNugget::Execute(Miner * pMiner)
{
	//the miner digs for gold until he is carrying in excess of MaxNuggets.
	//If he gets thirsty during his digging he stops work and
	//changes state to go to the saloon for a whiskey.
	pMiner->AddToGoldCarried(1);

	//diggin' is hard work
	pMiner->IncreaseFatigue();

	LOG( GetNameOfEntity(pMiner->ID()), ": ", "Pickin' up a nugget" );

	//if enough gold mined, go and put it in the bank
	if( pMiner->PocketsFull() )
	{
		pMiner->ChangeState( VisitBankAndDepositGold::Instance() );
	}

	//if thirsty go and get a whiskey
	if (pMiner->Thirsty())
	{
		pMiner->ChangeState( QuenchThirst::Instance() );
	}

}

void EnterMineAndDigForNugget::Exit(Miner * pMiner)
{
	LOG( GetNameOfEntity(pMiner->ID()), ": ", "Ah'm leavin' the gold mine with mah pockets full o' sweet gold");
}

void VisitBankAndDepositGold::Enter(Miner * pMiner)
{
	//on entry the miner makes sure he is located at the bank
	if (pMiner->Location() != bank)
	{
		LOG(GetNameOfEntity(pMiner->ID()), ": ", "Goin' to the bank. Yes siree");

		pMiner->ChangeLocation(bank);
	}
}

void VisitBankAndDepositGold::Execute(Miner * pMiner)
{
	//deposit the gold
	pMiner->DepositeToBank();

	LOG(GetNameOfEntity(pMiner->ID()), ": ", "Depositing gold. Total savings now: ", pMiner->Wealth());

	//wealthy enough to have a well earned rest?
	if (pMiner->IsComport())
	{
		LOG( GetNameOfEntity( pMiner->ID() ), ": ", "WooHoo! Rich enough for now. Back home to mah li'lle lady" );

		pMiner->ChangeState( GoHomeAndSleepTilRested::Instance() );
	}
	else
	{ //otherwise get more gold
		pMiner->ChangeState( EnterMineAndDigForNugget::Instance() );
	}
}

void VisitBankAndDepositGold::Exit(Miner * pMiner )
{
	LOG( GetNameOfEntity(pMiner->ID()), ": ", "Leavin' the bank" );
}

void QuenchThirst::Enter(Miner * pMiner )
{
	if (pMiner->Location() != saloon)
	{
		pMiner->ChangeLocation(saloon);

		LOG(GetNameOfEntity(pMiner->ID()), ": ", "Boy, ah sure is thusty! Walking to the saloon");
	}
}

void QuenchThirst::Execute(Miner * pMiner)
{
	if (pMiner->Thirsty())
	{
		pMiner->BuyAndDrinkAWhiskey();

		LOG( GetNameOfEntity(pMiner->ID()), ": ", "That's mighty fine sippin liquer" );

		pMiner->ChangeState(EnterMineAndDigForNugget::Instance());
	}
	else
	{
		int nRepeat(3);
		while ( nRepeat-- ) LOG_FILELINE_FN(" Error! ");
	}
}

void QuenchThirst::Exit(Miner * pMiner )
{
	LOG( GetNameOfEntity(pMiner->ID()), ": ", "Leaving the saloon, fellin' good" );
}

void GoHomeAndSleepTilRested::Enter(Miner * pMiner)
{
	if (pMiner->Location() != shack)
	{
		LOG( GetNameOfEntity(pMiner->ID()), ": ", "Walkin' home" );

		pMiner->ChangeLocation(shack);
	}
}

void GoHomeAndSleepTilRested::Execute(Miner * pMiner)
{
	//if miner is not fatigued start to dig for nuggets again.
	if (false == pMiner->Fatigued())
	{
		LOG( GetNameOfEntity( pMiner->ID()), ": ", "What a God darn fantastic nap! Time to find more gold" );

		pMiner->ChangeState(EnterMineAndDigForNugget::Instance());
	}
	else
	{
		//sleep
		pMiner->DecreaseFatigue();

		LOG( GetNameOfEntity(pMiner->ID()), ": ", "ZzzZzz ..." );
	}
}

void GoHomeAndSleepTilRested::Exit(Miner * pMiner)
{
	LOG( GetNameOfEntity(pMiner->ID()) , ": ", "Leaving the house");
}
