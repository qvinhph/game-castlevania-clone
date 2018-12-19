#include "SpawnerPanther.h"
#include "Monsters.h"
#include "Simon.h"
#include "Panther.h"



void CSpawnerPanther::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = right = x;
	top = bottom = y;
}

void CSpawnerPanther::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	if (spawning == true)
	{
		float x, y;
		int nx;
		SetupPantherInfo(x, y, nx);
		panther->SetPosition(x, y);
		panther->SetDirection(nx);
		panther->SetState(STATE_VISIBLE);
		spawning = false;
	}
	
	// Try to turn on the "spawning"
	else
	{
		if (this->IsInViewport() == false 
			&& panther->GetState() == STATE_INVISIBLE)
			spawning = true;
	}


	/*float vx, vy;
	this->panther->GetSpeed(vx, vy);
	if (this->IsInViewport() == false && vx == 0)
			this->panther->SetState(STATE_INVISIBLE);*/
}

void CSpawnerPanther::SetupPantherInfo(float & x, float & y, int & nx)
{
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	x = this->x;
	y = this->y;
	nx = (xS < x) ? -1 : 1;
}

CSpawnerPanther::CSpawnerPanther()
{
	panther = CMonsters::GetInstance()->Spawn(Monster::PANTHER);
	panther->SetState(STATE_VISIBLE);
}

