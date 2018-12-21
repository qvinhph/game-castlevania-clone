#include "ItemAxe.h"


void CItemAxe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);
	vy += GAME_GRAVITY * dt;				// simple fall down

	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);


	// Pick animation
	this->currentAniID = (int)ItemAxeAniID::IDLE;


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

void CItemAxe::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	right = x + ITEM_AXE_BBOX_HEIGHT;
	top = y;
	bottom = y + ITEM_AXE_BBOX_WIDTH;
}
