#include "ItemRope.h"

void CItemRope::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMovableObject::Update(dt);
	vy = GAME_FALL_SPEED;				// simple fall down

	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
		y += dy;
	else
		CMovableObject::ProceedCollisions(coEvents);
}

void CItemRope::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + ITEM_ROPE_BBOX_WIDTH;
	bottom = y + ITEM_ROPE_BBOX_HEIGHT;
}
