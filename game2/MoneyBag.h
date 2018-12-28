#pragma once
#include "MovableObject.h"

#define MONEY_BAG_BBOX_WIDTH		30
#define MONEY_BAG_BBOX_HEIGHT		30

#define RED_BAG_POINT_AWARD			100
#define WHITE_BAG_POINT_AWARD		700
#define BLUE_BAG_POINT_AWARD		400

enum class MoneyBagAniID
{
	RED_BAG = 2500,
	WHITE_BAG,
	BLUE_BAG,
};

class CMoneyBag : public CActiveObject
{
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

	CMoneyBag(int MoneyBagAniID);
};

