#include "SpawnerZombie.h"
#include "Simon.h"
#include "Zombie.h"
#include "Monsters.h"

DWORD CSpawnerZombie::rest_start = TIMER_IDLE;
DWORD CSpawnerZombie::pause_start = TIMER_IDLE;
int CSpawnerZombie::spawnCounter = 0;

void CSpawnerZombie::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + 1;
	bottom = top + 1;
}

void CSpawnerZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// If Simon is overlapping the SpawnerZombie, do spawn
	//if (this->IsOverlapping(CSimon::GetInstance()))
	//{
	//	// If not pausing spawning
	//	if (pause_start == TIMER_IDLE)
	//	{
	//		// If not resting
	//		if (rest_start == TIMER_IDLE)
	//		{
	//			// Spawn a zombie
	//			if (spawnCounter <= NUMBER_OF_ZOMBIES)
	//			{
	//				spawnCounter++;
	//				float x, y;
	//				int nx;
	//				SetupZombieInfo(x, y, nx);
	//				CMonsters::GetInstance()->Spawn(Monster::ZOMBIE, x, y, nx);

	//				// Start to rest spawning
	//				rest_start = GetTickCount();
	//			}
	//			else
	//			{
	//				// Start to pause spawning
	//				pause_start = GetTickCount();
	//				spawnCounter = 0;
	//			}
	//		}
	//		else
	//			if (GetTickCount() - rest_start > ZOMBIE_SPAWNER_REST_TIME)
	//				rest_start = TIMER_IDLE;
	//	}

	//	// Pausing spawning
	//	else
	//		if (GetTickCount() - pause_start > ZOMBIE_SPAWNER_PAUSE_TIME)
	//			pause_start = TIMER_IDLE;
	//}

	if (this->IsInViewport() && this->IsFarEnoughToSpawn())
	{
		// If not pausing spawning
		if (pause_start == TIMER_IDLE)
		{
			// If not resting
			if (rest_start == TIMER_IDLE)
			{
				// Spawn a zombie
				if (spawnCounter <= NUMBER_OF_ZOMBIES)
				{
					spawnCounter++;
					float x, y;
					int nx;
					SetupZombieInfo(x, y, nx);
					CMonsters::GetInstance()->Spawn(Monster::ZOMBIE, x, y, nx);

					// Start to rest spawning
					rest_start = GetTickCount();
				}
				else
				{
					// Start to pause spawning
					pause_start = GetTickCount();
					spawnCounter = 0;
				}
			}
			else
				if (GetTickCount() - rest_start > ZOMBIE_SPAWNER_REST_TIME)
					rest_start = TIMER_IDLE;
		}

		// Pausing spawning
		else
			if (GetTickCount() - pause_start > ZOMBIE_SPAWNER_PAUSE_TIME)
				pause_start = TIMER_IDLE;
	}
}

bool CSpawnerZombie::IsFarEnoughToSpawn()
{
	float xSimon, ySimon;
	CSimon::GetInstance()->GetPosition(xSimon, ySimon);

	if (abs(xSimon - this->x) > SAFE_SPACE_TO_SPAWN)
		return true;

	return false;
}

void CSpawnerZombie::SetupZombieInfo(float & x, float & y, int & nx)
{

	// Appear at the edge of the viewport
	float leftV, topV, rightV, bottomV;
	cameraInstance->GetBoundingBox(leftV, topV, rightV, bottomV);
	if (this->nx == 1)
		x = leftV;
	else
		x = rightV - ZOMBIE_BBOX_WIDTH;
	

	y = this->y;
	nx = this->nx;
}
