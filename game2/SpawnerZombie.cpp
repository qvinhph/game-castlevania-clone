#include "SpawnerZombie.h"
#include "Simon.h"
#include "Zombie.h"
#include "Monsters.h"

void CSpawnerZombie::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void CSpawnerZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// If Simon is overlapping the SpawnerZombie, do spawn
	if (this->IsOverlapping(CSimon::GetInstance()))
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

void CSpawnerZombie::SetupZombieInfo(float & x, float & y, int & nx)
{
	// Simon bounding box
	float leftS, topS, rightS, bottomS;
	CSimon::GetInstance()->GetBoundingBox(leftS, topS, rightS, bottomS);

	// Viewport bounding box
	float leftV, topV, rightV, bottomV;
	cameraInstance->GetBoundingBox(leftV, topV, rightV, bottomV);

	// TO-DO: Setup a random nx
	// Because somehow zombie may appear from the left of the viewport
	// And the probability of that case seem to be less.
	nx = -1;

	if (nx != 1)
	{
		x = rightV - ZOMBIE_BBOX_WIDTH;
		y = bottomS - ZOMBIE_BBOX_HEIGHT;
	}
}

CSpawnerZombie::CSpawnerZombie(float width, float height) : width(width), height(height)
{
	pause_start = TIMER_IDLE;
	rest_start = TIMER_IDLE;
	spawnCounter = 0;
}
