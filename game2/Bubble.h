#pragma once
#include "MovableObject.h"

#define BUBBLE_BBOX_WIDTH		14
#define BUBBLE_BBOX_HEIGHT		28
#define BUBBLE_FALL_GRAVITY		0.0008f

enum class BubbleAniID
{
	IDLE = 2000,
};

class CBubble : public CActiveObject
{

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> * coObjects = NULL) override;

};

