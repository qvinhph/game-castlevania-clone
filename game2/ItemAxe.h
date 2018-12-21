#pragma once
#include "MovableObject.h"

#define ITEM_AXE_BBOX_WIDTH		30
#define ITEM_AXE_BBOX_HEIGHT	28

enum class ItemAxeAniID
{
	IDLE = 2700,
};

class CItemAxe : public CActiveObject
{

public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

};

