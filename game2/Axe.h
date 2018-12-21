#pragma once
#include "MovableObject.h"

#define AXE_BBOX_WIDTH		30
#define AXE_BBOX_HEIGHT		28

#define AXE_INITIAL_SPEED_VX	0.2f
#define AXE_INITIAL_SPEED_VY	-0.6f
#define AXE_GRAVITY				0.0016f

#define AXE_HEART_COST		1		// When use
#define AXE_DAMAGE			2

enum class AxeAniID
{
	FLYING = 2701,
};

class CAxe : public CActiveObject
{
public:
	
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) override;
	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;
	void SetState(int state) override;

	CAxe();
};

