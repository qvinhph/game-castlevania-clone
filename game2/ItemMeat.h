#pragma once
#include "MovableObject.h"

#define ITEM_MEAT_BBOX_WIDTH		32
#define ITEM_MEAT_BBOX_HEIGHT		26
#define ITEM_MEAT_HEALTH_AWARD		4

enum class ItemMeatAniID
{
	IDLE = 2300,
};

class CItemMeat : public CActiveObject
{
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

	CItemMeat();
};

