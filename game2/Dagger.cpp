#include "Dagger.h"
#include "Weapons.h"
#include "InvisibleWall.h"
#include "BigCandle.h"
#include "Candle.h"
#include "Panther.h"
#include "Zombie.h"
#include "PinkBat.h"
#include "BossBat.h"
#include "Fish.h"


void CDagger::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + DAGGER_BBOX_WIDTH;
	bottom = top + DAGGER_BBOX_HEIGHT;
}

void CDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);

	// Choose speed depend on direction
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

	// If the dagger go out the viewport
	if (this->IsInViewport() == false)
		SetState(STATE_INVISIBLE);		
}

void CDagger::ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents)
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
			dynamic_cast<CZombie *>(e->obj) ||
			dynamic_cast<CPinkBat *>(e->obj) ||
			dynamic_cast<CFish *>(e->obj) ||
			dynamic_cast<CBossBat *>(e->obj))
		{
			e->obj->BeHit(this->damage);
			this->SetState(STATE_INVISIBLE);
			break;
		}
		else
		{
			// Ignore other objects by completing the rest of dx / dy
			if (e->nx != 0)	x += (1 - min_tx) * dx;
			if (e->ny != 0)	y += (1 - min_ty) * dy;
		}
	}
}

void CDagger::SetState(int state)
{
	CGameObject::SetState(state);

	// Make the dagger object available again to use next time
	if (state == STATE_INVISIBLE)
		CWeapons::GetInstance()->AddQuantity(Weapon::DAGGER);
}

CDagger::CDagger()
{
	damage = DAGGER_DAMAGE;
}


