#include "Cross.h"


void CCross::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);
	vy += GAME_GRAVITY * dt;				// simple fall down

	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);


	// Pick animation
	currentAniID = (int)CrossAniID::IDLE;


	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
		CActiveObject::ProceedCollisions(coEvents);


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CCross::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + CROSS_BBOX_WIDTH;
	bottom = y + CROSS_BBOX_HEIGHT;
}
