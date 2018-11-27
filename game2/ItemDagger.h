#pragma once
#include "MovableObject.h"

#define ITEMDAGGER_BBOX_HEIGHT		18.0f
#define ITEMDAGGER_BBOX_WIDTH		32.0f

enum class ItemDaggerAniID
{
	IDLE = 1800
};

class CItemDagger : public CActiveObject
{

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) override;

	CItemDagger() { this->currentAniID = (int)ItemDaggerAniID::IDLE; }
};

