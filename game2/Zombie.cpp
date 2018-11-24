#include "Zombie.h"
#include "InvisibleWall.h"
#include "Simon.h"

#include "debug.h"


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

	// Simple fall down
	vy += GAME_GRAVITY * dt;


	// Choose animation
	currentAniID = (nx > 0) ?
		(int)ZombieAniID::WALK_RIGHT :
		(int)ZombieAniID::WALK_LEFT;


	// Update speed 
	vx = (nx > 0) ?
		ZOMBIE_WALKING_SPEED :
		-ZOMBIE_WALKING_SPEED;


	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else ProceedCollisions(coEvents);


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CZombie::ProceedCollisions(vector<LPCOLLISIONEVENT>& coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;
	float min_tx, min_ty, nx, ny;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	// Set the object's position right to the point occured collision
	x += min_tx * dx;
	y += min_ty * dy;

	for (UINT i = 0; i < coEventsResult.size(); ++i)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		if (dynamic_cast<CSimon *>(e->obj))
			DebugOut(L"\nadssadf");

		if (dynamic_cast<CInvisibleWall *>(e->obj))
		{
			if (e->ny != 0)
			{
				vy = 0;
				y += DEFLECTION_AVOID_OVERLAPPING * e->ny;
			}
		}
		else
		{
			// Ignore other objects by completing the rest of dx / dy
			if (e->nx != 0)	x += (1 - min_tx) * dx;
			if (e->ny < 0)	y += (1 - min_ty) * dy;
		}
	}
}
