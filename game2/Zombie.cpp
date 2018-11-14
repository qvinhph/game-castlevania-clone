#include "Zombie.h"


void CZombie::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + ZOMBIE_BBOX_WIDTH;
	bottom = top + ZOMBIE_BBOX_HEIGHT;
}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMovableObject::Update(dt);

	currentAniID = (nx > 0) ?
		(int)ZombieAniID::WALK_RIGHT :
		(int)ZombieAniID::WALK_LEFT;

	vx = (nx > 0) ?
		ZOMBIE_WALKING_SPEED :
		-ZOMBIE_WALKING_SPEED;

	vy += GAME_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
		ProceedCollisions(coEvents);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
