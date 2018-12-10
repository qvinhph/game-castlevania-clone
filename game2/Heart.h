#pragma once
#include "MovableObject.h"

#define HEART_BBOX_WIDTH					16.0f
#define HEART_BBOX_HEIGHT					16.0f

#define HEART_FALL_SPEED_Y					0.04
#define HEART_VECLOCITY_X					0.0002
#define HEART_MAX_SPEED_X_ALLOWED			0.1f			

enum class HeartAniID
{
	IDLE = 1700
};

class CHeart : public CActiveObject
{

	float velocity_x;		// use for making heart's unique movement (falling down) 

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void SetState(int state) override;
	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;

	CHeart();
};

