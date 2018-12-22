#include "Heart.h"
#include "debug.h"
#include "InvisibleWall.h"


void CHeart::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + HEART_BBOX_WIDTH;
	bottom = top + HEART_BBOX_HEIGHT;
}

void CHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);

	// Simple vertical movement
	vy = HEART_FALL_SPEED_Y;
	
	// Horizontal movement
	vx += velocity_x * dt;
	if (abs(vx) > HEART_MAX_SPEED_X_ALLOWED &&
		vx * velocity_x > 0)						// and vx is increasing
		velocity_x = -velocity_x;

	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		ProceedCollisions(coEvents);
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	// If the heart go out the viewport
	if (this->IsInViewport() == false)
		SetState(STATE_INVISIBLE);
}

void CHeart::SetState(int state)
{
	CGameObject::SetState(state);

	// Reset the vx, vy every time the Heart is set to VISIBLE
	// to keep its unique movement the same 
	if (state == STATE_VISIBLE)
	{
		vy = HEART_FALL_SPEED_Y;
		vx = HEART_MAX_SPEED_X_ALLOWED;
	}
}

void CHeart::ProceedCollisions(vector<LPCOLLISIONEVENT>& coEvents)
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

		if (dynamic_cast<CInvisibleWall *>(e->obj))
		{
			// Block when reach the ground
			if (e->ny < 0)
			{
				y += DEFLECTION_AVOID_OVERLAPPING * e->ny;
				vy = 0;
			}

			vx = vy = 0;
		}
		else
		{
			// Ignore other objects by completing the rest of dx / dy
			if (e->nx != 0)	x += (1 - min_tx) * dx;
			if (e->ny != 0)	y += (1 - min_ty) * dy;
		}
	}
}

CHeart::CHeart()
{
	point = HEART_POINT_AWARD;

	this->velocity_x = HEART_VECLOCITY_X;
	this->currentAniID = (int)HeartAniID::IDLE;

	vy = HEART_FALL_SPEED_Y;
	vx = HEART_MAX_SPEED_X_ALLOWED;
}