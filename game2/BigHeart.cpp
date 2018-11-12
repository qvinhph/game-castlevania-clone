#include "BigHeart.h"

void CBigHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMovableObject::Update(dt);
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
		CMovableObject::ProceedCollisions(coEvents);
}

void CBigHeart::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = BIGHEART_BBOX_WIDTH + x;
	bottom = BIGHEART_BBOX_HEIGHT + y;
}
