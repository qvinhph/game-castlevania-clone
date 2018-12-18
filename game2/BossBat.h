#pragma once
#include "MovableObject.h"

#define BOSSBAT_BBOX_WIDTH		96
#define BOSSBAT_BBOX_HEIGHT		44

#define BOSSBAT_REST_TIME		1500
#define BOSSBAT_ATTACK_TIME		1000

#define BOSSBAT_GO_TO_REST_SPEED_VX		0.06f
#define BOSSBAT_GO_TO_REST_SPEED_VY		0.06f

#define BOSSBAT_MAX_SPEED_VX			0.2f
#define BOSSBAT_MAX_SPEED_VY			0.3f
#define BOSSBAT_FLY_UP_SPEED_VY			-0.004f

// Minimum space between Simon and the boss while it is resting
#define BOSSBAT_MINIMUM_DISTANCE_WITH_SIMON		150


enum BossBatAniID
{
	SLEEP,
	FLY,
};

class CBossBat : public CActiveObject
{
	bool sleeping = true;
	bool goingToRest = false;

	float central_x;		// Position of the the boss
	float central_y;

	float target_x;			// Position of the player
	float target_y;

	float rest_x;			// Position of the boss is going to rest
	float rest_y;		
	
	// Timers, also be used as FLAG for the action when assigned to TIMER_IDLE(false) or non-TIMER_IDLE(true)
	// Assigned to TIMER_IDLE			: the timer is not working,
	// Assigned to TIMER_ETERNAL		: the timer is working and the action is not going to stop
	// Assigned to unsigned number		: the timer is working
	DWORD rest_start = TIMER_IDLE;
	DWORD attack_start = TIMER_IDLE;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	bool IsInViewport() override;
	void ProceedAttacking();

	void CalculateCentralPosition();
	void CalculateRestPlace();
	void FindTarget();
	bool CanWakeUp();
	bool ReachedRestPlace();


	void StartAttack();
	void GoToRest();
	void Rest();
};

