#pragma once
#include "MovableObject.h"

#define ITEM_HOLY_WATER_WIDTH		32
#define ITEM_HOLY_WATER_HEIGHT		32

enum class ItemHolyWaterAniID
{
	IDLE = 2800,
};

class CItemHolyWater : public CActiveObject
{
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

};

