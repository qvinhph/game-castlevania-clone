#include "HolyWater.h"
#include "Weapons.h"
#include "BigCandle.h"
#include "Candle.h"
#include "Zombie.h"
#include "PinkBat.h"
#include "Fish.h"
#include "Panther.h"
#include "InvisibleWall.h"
#include "BossBat.h"

void CHolyWater::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	if (currentAniID == (int)HolyWaterAniID::EXPLODE)
	{
		right = left + HOLYWATER_EXPLODE_BBOX_WIDTH;
		bottom = top + HOLYWATER_EXPLODE_BBOX_HEIGHT;
	}
	else
	{
		right = left + HOLYWATER_IDLE_BBOX_WIDTH;
		bottom = top + HOLYWATER_IDLE_BBOX_HEIGHT;
	}
}

void CHolyWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);


	// Falling down
	if (exploding == false)
		vy += HOLYWATER_IDLE_GRAVITY * this->dt;


	// Choose animation 
	PickAnimation();


	// Exploding
	if (explode_start != TIMER_IDLE)
		if (GetTickCount() - explode_start > HOLYWATER_EXPLODE_TIME)
		{
			explode_start = TIMER_IDLE;
			this->SetState(STATE_INVISIBLE);
		}


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

void CHolyWater::ProceedCollisions(vector<LPCOLLISIONEVENT>& coEvents)
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
		}
		else if (dynamic_cast<CInvisibleWall *>(e->obj))
		{
			vx = vy = 0;
			if (exploding == false)
			{
				// Calibrate the position of the fire
				y += HOLYWATER_IDLE_BBOX_HEIGHT - HOLYWATER_EXPLODE_BBOX_HEIGHT;
				x += (HOLYWATER_IDLE_BBOX_WIDTH - HOLYWATER_EXPLODE_BBOX_WIDTH) / 2;

				// Catch a fire
				exploding = true;
				explode_start = GetTickCount();
			}
		}
	}
}

void CHolyWater::SetState(int state)
{
	CActiveObject::SetState(state);

	if (state == STATE_VISIBLE)
	{
		vx = (nx > 0) ? 
			HOLYWATER_IDLE_INITIAL_SPEED_VX : 
			-HOLYWATER_IDLE_INITIAL_SPEED_VX;

		vy = HOLYWATER_IDLE_INITIAL_SPEED_VY;

		exploding = false;
	}

	// Make the axe object available again to use next time
	else if (state == STATE_INVISIBLE)
		CWeapons::GetInstance()->AddQuantity(Weapon::HOLYWATER);
}

void CHolyWater::PickAnimation()
{
	if (exploding)
	{
		currentAniID = (int)HolyWaterAniID::EXPLODE;
	}
	else
	{
		currentAniID = (nx > 0) ?
			(int)HolyWaterAniID::IDLE_RIGHT :
			(int)HolyWaterAniID::IDLE_LEFT;
	}
}

CHolyWater::CHolyWater()
{
	this->damage = HOLYWATER_DAMAGE;
}
