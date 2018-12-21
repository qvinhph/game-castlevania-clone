#pragma once
#include "MovableObject.h"

#define HOLYWATER_IDLE_BBOX_HEIGHT			16
#define HOLYWATER_IDLE_BBOX_WIDTH			16
#define HOLYWATER_EXPLODE_BBOX_HEIGHT		26
#define HOLYWATER_EXPLODE_BBOX_WIDTH		32

#define HOLYWATER_IDLE_GRAVITY				0.0007f
#define HOLYWATER_IDLE_INITIAL_SPEED_VX		0.15f
#define HOLYWATER_IDLE_INITIAL_SPEED_VY		-0.08f

#define HOLYWATER_EXPLODE_TIME				900
#define HOLYWATER_DAMAGE					2

enum class HolyWaterAniID
{
	IDLE_RIGHT = 2801,
	IDLE_LEFT,
	EXPLODE,
};

class CHolyWater : public CActiveObject
{

	bool exploding = false;

	// Timers, also be used as FLAG for the action when assigned to TIMER_IDLE(false) or non-TIMER_IDLE(true)
	// Assigned to TIMER_IDLE			: the timer is not working,
	// Assigned to TIMER_ETERNAL		: the timer is working and the action is not going to stop
	// Assigned to unsigned number		: the timer is working
	DWORD explode_start = TIMER_IDLE;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) override;
	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;
	void SetState(int state) override;
	void PickAnimation();

	CHolyWater();

};

