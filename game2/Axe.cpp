#include "Axe.h"
#include "Weapons.h"
#include "Zombie.h"
#include "Fish.h"
#include "Candle.h"
#include "BigCandle.h"
#include "Panther.h"


void CAxe::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + AXE_BBOX_WIDTH;
	bottom = y + AXE_BBOX_HEIGHT;
}

void CAxe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);

	// Falling down
	vy += AXE_GRAVITY * this->dt;


	// Choose animation 
	currentAniID = (int)AxeAniID::FLYING;


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

	// If the dagger go out the viewport
	if (this->IsInViewport() == false)
		SetState(STATE_INVISIBLE);
}

void CAxe::ProceedCollisions(vector<LPCOLLISIONEVENT>& coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;
	float nx, ny, min_tx, min_ty;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	// Set the object's position right to the point occured collision
	x += min_tx * dx;
	y += min_ty * dy;

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (dynamic_cast<CBigCandle *>(e->obj) ||
			dynamic_cast<CCandle *>(e->obj) ||
			dynamic_cast<CPanther *>(e->obj) ||
			dynamic_cast<CZombie *>(e->obj))
		{
			e->obj->BeHit(this->damage);
		}
		else
		{
			// Ignore other objects by completing the rest of dx / dy
			if (e->nx != 0)	x += (1 - min_tx) * dx;
			if (e->ny != 0)	y += (1 - min_ty) * dy;
		}
	}
}

void CAxe::SetState(int state)
{
	CActiveObject::SetState(state);

	if (state == STATE_VISIBLE)
	{
		vx = (nx > 0) ? AXE_INITIAL_SPEED_VX : -AXE_INITIAL_SPEED_VX;
		vy = AXE_INITIAL_SPEED_VY;
	}

	// Make the axe object available again to use next time
	else if (state == STATE_INVISIBLE)
		CWeapons::GetInstance()->AddQuantity(Weapon::AXE);
}
