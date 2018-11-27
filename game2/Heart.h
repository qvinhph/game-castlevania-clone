#pragma once
#include "MovableObject.h"

#define HEART_BBOX_WIDTH					16.0f
#define HEART_BBOX_HEIGHT					16.0f

#define HEART_FALL_SPEED_Y					0.03f			 // 
#define HEART_VECLOCITY_X					0.00015f		 // DELETE ME: these number work fine
#define HEART_MAX_SPEED_X_ALLOWED			0.1f			 // 

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

	CHeart();
};

