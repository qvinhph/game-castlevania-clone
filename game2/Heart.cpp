#include "Heart.h"
#include "debug.h"


void CHeart::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + HEART_BBOX_WIDTH;
	bottom = top + HEART_BBOX_HEIGHT;
}

void CHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMovableObject::Update(dt);

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
		vx = vy = 0;
		CMovableObject::ProceedCollisions(coEvents);
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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

CHeart::CHeart()
{
	this->velocity_x = HEART_VECLOCITY_X;
	this->currentAniID = (int)HeartAniID::IDLE;

	vy = HEART_FALL_SPEED_Y;
	vx = HEART_MAX_SPEED_X_ALLOWED;
}