#include "Dagger.h"
#include "Weapons.h"
#include "InvisibleWall.h"
#include "BigCandle.h"



void CDagger::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + DAGGER_BBOX_WIDTH;
	bottom = top + DAGGER_BBOX_HEIGHT;
}

void CDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMovableObject::Update(dt);

	// Choose directon of speed 
	vx = (nx > 0) ?
		DAGGER_SPEED_X :
		-DAGGER_SPEED_X;

	// Choose animation matches the direction
	currentAniID = (nx > 0) ?
		(int)DaggerAniID::IDLE_RIGHT :
		(int)DaggerAniID::IDLE_LEFT;

	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
		ProceedCollisions(coEvents);

	// Clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CDagger::ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;
	float nx, ny, min_tx, min_ty;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	// If there is no collision
	if (min_tx == 1) x += dx;
	if (min_ty == 1) y += dy;

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (dynamic_cast<CBigCandle *>(e->obj))
		{
			e->obj->Destroy();
			this->SetState(STATE_INVISIBLE);
			break;
		}
	}
}

void CDagger::SetState(int state)
{
	CGameObject::SetState(state);

	// Make the dagger object available again to use next time
	if (state == STATE_INVISIBLE)
		CWeapons::GetInstance()->AddToStock(Weapon::DAGGER);
}


