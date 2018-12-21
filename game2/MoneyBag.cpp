#include "MoneyBag.h"

void CMoneyBag::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

void CMoneyBag::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + MONEY_BAG_BBOX_WIDTH;
	bottom = top + MONEY_BAG_BBOX_HEIGHT;
}

CMoneyBag::CMoneyBag(int MoneyBagAniID)
{
	if (MoneyBagAniID == MoneyBagAniID::BLUE_BAG)
	{
		currentAniID = MoneyBagAniID::BLUE_BAG;
		point = BLUE_BAG_POINT_AWARD;
	}

	else if (MoneyBagAniID == MoneyBagAniID::WHITE_BAG)
	{
		currentAniID = MoneyBagAniID::WHITE_BAG;
		point = WHITE_BAG_POINT_AWARD;
	}

	else if (MoneyBagAniID == MoneyBagAniID::RED_BAG)
	{
		currentAniID = MoneyBagAniID::RED_BAG;
		point = RED_BAG_POINT_AWARD;
	}
}
