#include "ItemMeat.h"



void CItemMeat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

void CItemMeat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + ITEM_MEAT_BBOX_WIDTH;
	bottom = top + ITEM_MEAT_BBOX_HEIGHT;
}

CItemMeat::CItemMeat()
{
	this->currentAniID = (int)ItemMeatAniID::IDLE;
	this->point = ITEM_MEAT_HEALTH_AWARD;
}
