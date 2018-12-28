#pragma once
#include "MovableObject.h"

#define MAGICBALL_BBOX_WIDTH	28
#define MAGICBALL_BBOX_HEIGHT	32

#define MAGICBALL_POINT_AWARD	16

enum class MagicBallAniID
{
	TWINKLE = 3200,
};

class CMagicBall : public CActiveObject
{
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

	CMagicBall() { this->point = MAGICBALL_POINT_AWARD; }
};

