#pragma once
#include "GameObject.h"

#define GAME_GRAVITY						0.002
#define DEFLECTION_AVOID_OVERLAPPING		0.4f	// Use when two objects collide, avoid overlapping next frame
#define UNTOUCHABLE_TIME					200

enum class Action
{
	IDLE,
	WALK_RIGHT,
	WALK_LEFT,
	CROUCH,
	JUMP,
	UPSTAIRS,
	DOWNSTAIRS,
	ATTACK,
	USE_ITEM,
	GO_IN,
	GET_DAMAGED,
	DIE,
};


/*
	The object that need be updated every frame
*/
class CActiveObject : public CGameObject
{
protected:

	float dx;
	float dy;

	float vx;
	float vy;

	DWORD dt;


	// Timers, also be used as FLAG for the action when assigned to TIMER_IDLE(false) or non-TIMER_IDLE(true)
	// Assigned to TIMER_IDLE			: the timer is not working,
	// Assigned to TIMER_ETERNAL		: the timer is working and the action is not going to stop
	// Assigned to unsigned number		: the timer is working
	DWORD untouchable_start = TIMER_IDLE;

public:

	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx, vy = this->vy; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents,
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny);
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);	
	virtual void BeHit(int damage) override;
	virtual void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents);
	virtual void ProceedBeingUntouchable();

	CActiveObject();
};

typedef CActiveObject * LPACTIVEOBJECT;

