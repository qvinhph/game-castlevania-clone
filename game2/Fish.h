#pragma once
#include "MovableObject.h"
#include "Weapons.h"

#define FISH_BBOX_WIDTH				32
#define FISH_BBOX_HEIGHT			62

#define FISH_MOVE_SPEED				0.09f
#define FISH_JUMP_SPEED				0.7f
#define FISH_JUMP_GRAVITY			0.0008f
#define FISH_ATTACK_TIME			500
#define FISH_PAUSE_ATTACK_TIME		2500

#define FISH_POINT_AWARD			300
#define FISH_DEFAULT_HEALTH			1
#define FISH_DAMAGE					2


enum class FishAniID
{
	IDLE_RIGHT = 5400,
	IDLE_LEFT,
	
	MOVE_RIGHT,
	MOVE_LEFT,
	
	ATTACK_RIGHT,
	ATTACK_LEFT,
};

class CFish: public CActiveObject
{
	bool firstAppear = true;
	bool jumping = false;
	Weapon fireball = Weapon::FIREBALL;

	// Timers, also be used as FLAG for the action when assigned to TIMER_IDLE(false) or non-TIMER_IDLE(true)
	// Assigned to TIMER_IDLE			: the timer is not working,
	// Assigned to TIMER_ETERNAL		: the timer is working and the action is not going to stop
	// Assigned to unsigned number		: the timer is working
	DWORD attack_start = TIMER_IDLE;
	DWORD attack_pause = TIMER_IDLE;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;

	void SetFirstAppear(bool firstAppear) { this->firstAppear = firstAppear; }
	bool GetFirstAppear() { return this->firstAppear; }

	void Jump();
	void Attack();
	void Move();

	void ProceedAttacking();
	void ProceedPausingAttacking();
	void PickAnimation();

	CFish();

};

