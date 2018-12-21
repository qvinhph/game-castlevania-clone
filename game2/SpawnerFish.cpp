#include "SpawnerFish.h"
#include "Simon.h"
#include "Monsters.h"
#include "Bubbles.h"


void CSpawnerFish::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + FISH_BBOX_WIDTH;
	bottom = y + FISH_BBOX_HEIGHT;
}

void CSpawnerFish::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (IsReadyToSpawn() && spawning_pause_start == TIMER_IDLE)
	{
		// Get the fish and its information
		LPGAMEOBJECT fish = NULL;


		fish = CMonsters::GetInstance()->Spawn(Monster::FISH);
		if (fish != NULL)
		{
			SetupFish((CFish *)fish);
			spawning_pause_start = GetTickCount();
		}
			
	}

	// Counting pausing time
	if (spawning_pause_start != TIMER_IDLE)
	{
		if (GetTickCount() - spawning_pause_start > FISH_SPAWNER_PAUSE_TIME)
		{
			spawning_pause_start = TIMER_IDLE;
		}
	}
}

void CSpawnerFish::SetupFish(CFish * const & fish)
{
	float xSimon, ySimon;
	CSimon::GetInstance()->GetPosition(xSimon, ySimon);

	CBubbles::GetInstance()->ShowSomeBubblesForFish(this->x, this->y);
	fish->SetPosition(this->x, this->y);
	fish->ReDirect(xSimon);
	fish->SetSpeed(0, 0);
	fish->SetFirstAppear(true);
	fish->SetState(STATE_VISIBLE);
}

bool CSpawnerFish::IsReadyToSpawn()
{
	// Get Simon's central position
	float left, top, right, bottom;
	CSimon::GetInstance()->GetBoundingBox(left, top, right, bottom);
	float simonCentralX = (left + right) / 2;

	if (abs(simonCentralX - x) > SUITABLE_DISTANCE_FROM_SIMON_TO_SPAWN)
		return true;

	return false;
}
