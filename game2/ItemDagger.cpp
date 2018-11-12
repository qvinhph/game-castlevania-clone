#include "ItemDagger.h"
#include "debug.h"


void CItemDagger::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + ITEMDAGGER_BBOX_WIDTH;
	bottom = top + ITEMDAGGER_BBOX_HEIGHT;
}

void CItemDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMovableObject::Update(dt);

	vy += GAME_GRAVITY * dt;

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
