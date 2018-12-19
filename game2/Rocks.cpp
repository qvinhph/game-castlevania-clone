#include "Rocks.h"

CRocks * CRocks::__instance = NULL;

void CRocks::Add(CRock * rock)
{
	rock->SetState(STATE_INVISIBLE);
	rocks.push_back(rock);
}

void CRocks::ShowSomeRocksAfterBreaking(float x, float y)
{
	CRock * rock = NULL;
	UINT i = 0;

	// Get rock no.1
	for (; i < rocks.size(); i++)
	{
		if (rocks[i]->GetState() == STATE_INVISIBLE)
		{
			rock = rocks[i];
			i++;
			break;
		}
	}
	if (rock != NULL)
	{
		rock->SetPosition(x, y);
		rock->SetSpeed(ROCK1_INITIAL_SPEED_X, ROCK1_INITIAL_SPEED_Y);
		rock->SetState(STATE_VISIBLE);
	}


	// Get rock no.2
	for (; i < rocks.size(); i++)
	{
		if (rocks[i]->GetState() == STATE_INVISIBLE)
		{
			rock = rocks[i];
			i++;
			break;
		}
	}	
	if (rock != NULL)
	{
		rock->SetPosition(x, y);
		rock->SetSpeed(ROCK2_INITIAL_SPEED_X, ROCK2_INITIAL_SPEED_Y);
		rock->SetState(STATE_VISIBLE);
	}

	// Get rock no.3
	for (; i < rocks.size(); i++)
	{
		if (rocks[i]->GetState() == STATE_INVISIBLE)
		{
			rock = rocks[i];
			i++;
			break;
		}
	}
	if (rock != NULL)
	{
		rock->SetPosition(x, y);
		rock->SetSpeed(ROCK3_INITIAL_SPEED_X, ROCK3_INITIAL_SPEED_Y);
		rock->SetState(STATE_VISIBLE);
	}
	
	// Get rock no.4
	for (; i < rocks.size(); i++)
	{
		if (rocks[i]->GetState() == STATE_INVISIBLE)
		{
			rock = rocks[i];
			i++;
			break;
		}
	}
	if (rock != NULL)
	{
		rock->SetPosition(x, y);
		rock->SetSpeed(ROCK4_INITIAL_SPEED_X, ROCK4_INITIAL_SPEED_Y);
		rock->SetState(STATE_VISIBLE);
	}

}

CRocks * CRocks::GetInstance()
{
	if (__instance == NULL)
		__instance = new CRocks();

	return __instance;
}
