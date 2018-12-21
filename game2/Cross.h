#pragma once
#include "MovableObject.h"

#define CROSS_BBOX_HEIGHT		32
#define CROSS_BBOX_WIDTH		32

enum class CrossAniID
{
	IDLE = 2600,
};

class CCross : public CActiveObject
{
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
};

