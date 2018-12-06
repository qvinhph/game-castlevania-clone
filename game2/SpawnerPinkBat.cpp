#include "SpawnerPinkBat.h"
#include "Simon.h"
#include "Monsters.h"
#include "PinkBat.h"


void CSpawnerPinkBat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;
}

void CSpawnerPinkBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->IsOverlapping(CSimon::GetInstance()))
	{
		if (spawn_pause == TIMER_IDLE)
		{
			float x, y, line;
			int nx;
			SetupBatInfo(x, y, nx, line);
			LPACTIVEOBJECT bat = CMonsters::GetInstance()->Spawn(Monster::PINKBAT, x, y, nx);
			dynamic_cast<CPinkBat *>(bat)->SetLine(line);

			// Start the timer
			spawn_pause = GetTickCount();
		}

		else
		{
			if (GetTickCount() - spawn_pause > PAUSE_TIME)
				spawn_pause = TIMER_IDLE;
		}			
	}
}

void CSpawnerPinkBat::SetupBatInfo(float & x, float & y, int & nx, float & line)
{
	// Set nx
	LPGAMEOBJECT simon = CSimon::GetInstance();
	nx = -simon->GetDirection();


	// Set y
	float left, top, right, bottom;
	simon->GetBoundingBox(left, top, right, bottom);
	y = (bottom + top) / 2;


	// Set x
	float leftV, topV, rightV, bottomV;
	CGame::GetInstance()->GetViewportBoundingBox(leftV, topV, rightV, bottomV);
	x = (nx > 0) ?
		leftV : rightV - BAT_BBOX_WIDTH;


	// Set the line 
	line = (top + bottom) / 2;

}

CSpawnerPinkBat::CSpawnerPinkBat(float width, float height)
	:width(width), height(height)
{
}

