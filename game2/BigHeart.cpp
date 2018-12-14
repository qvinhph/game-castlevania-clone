#include "BigHeart.h"

void CBigHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);
	vy += GAME_GRAVITY * dt;				// simple fall down

	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

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

CBigHeart::CBigHeart()
{
	this->currentAniID = (int)BigHeartAniID::IDLE;
	this->point = BIG_HEART_POINT_AWARD;
}

void CBigHeart::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = BIGHEART_BBOX_WIDTH + x;
	bottom = BIGHEART_BBOX_HEIGHT + y;
}
