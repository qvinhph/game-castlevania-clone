#pragma once
#include "MovableObject.h"

#define FIRE_BALL_BBOX_WIDTH		14
#define FIRE_BALL_BBOX_HEIGHT		10

#define FIRE_BALL_SPEED_VX			0.18

enum FireBallAniID
{
	FLYING_LEFT = 1900,
	FLYING_RIGHT,
};

class CFireBall : public CActiveObject
{

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;

	void SetState(int state) override;
	CFireBall();

};

