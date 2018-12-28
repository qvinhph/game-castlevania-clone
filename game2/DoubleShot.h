#pragma once
#include "MovableObject.h"

#define DOUBLESHOT_BBOX_WIDTH	28	
#define DOUBLESHOT_BBOX_HEIGHT	28

enum class DoubleShotAniID
{
	IDLE = 3100,
};

class CDoubleShot: public CActiveObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

};

