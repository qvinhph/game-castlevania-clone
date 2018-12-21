#pragma once 
#include "GameObject.h"

#define SECRECT_BRICK_BBOX_HEIGHT		32
#define SECRECT_BRICK_BBOX_WIDTH		32

#define CANNOT_BE_HIT_TIME				200

enum class SecretBrickAniID
{
	IDLE_TYPE_1 = 2100,
	IDLE_TYPE_2,
};

class CSecretBrick : public CGameObject
{
	// To help that only one secret brick can be hit at a time
	// Timers, also be used as FLAG for the action when assigned to TIMER_IDLE(false) or non-TIMER_IDLE(true)
	// Assigned to TIMER_IDLE			: the timer is not working,
	// Assigned to TIMER_ETERNAL		: the timer is working and the action is not going to stop
	// Assigned to unsigned number		: the timer is working
	static DWORD cannot_be_hit_start;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Destroy() override;
	void BeHit(int damage) override;

	CSecretBrick(int currentAniID);
};

