#include "ItemDagger.h"
#include "debug.h"


void CItemDagger::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + DAGGER_BBOX_WIDTH;
	bottom = top + DAGGER_BBOX_HEIGHT;
}

void CItemDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMovableObject::Update(dt);

	vy = GAME_FALL_SPEED;

	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
		x += dx;
	else
		CMovableObject::ProceedCollisions(coEvents);


}

CItemDagger::CItemDagger()
{
	this->currentAniID = (int)ItemDaggerAniID::IDLE;
	this->action = Action::ITEM_FORM;
}
