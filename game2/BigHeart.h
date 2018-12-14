#pragma once
#include "MovableObject.h"

#define BIGHEART_BBOX_WIDTH		24
#define BIGHEART_BBOX_HEIGHT	20

#define BIG_HEART_POINT_AWARD	5


enum class BigHeartAniID
{
	IDLE = 1600
};

class CBigHeart : public CActiveObject
{
public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	
	CBigHeart();
};

