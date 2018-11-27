#pragma once
#include "MovableObject.h"

#define DAGGER_BBOX_HEIGHT		18.0f
#define DAGGER_BBOX_WIDTH		32.0f
#define DAGGER_SPEED_X			0.4f

enum class DaggerAniID
{
	IDLE_LEFT = 1810,
	IDLE_RIGHT,
};

class CDagger: public CActiveObject
{
public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) override;
	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;
	void SetState(int state) override;

};

