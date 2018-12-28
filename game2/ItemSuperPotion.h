#pragma once
#include "MovableObject.h"

#define ITEM_SUPER_POTION_BBOX_HEIGHT		32.0f
#define ITEM_SUPER_POTION_BBOX_WIDTH		26.0f

enum class ItemSuperPotionAniID
{
	IDLE = 2900,
};

class CItemSuperPotion : public CActiveObject
{
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
};

