#pragma once
#include "GameObject.h"

#define GAME_GRAVITY						0.002
#define DEFLECTION_AVOID_OVERLAPPING		0.4f	// Use when two objects collide, avoid overlapping next frame

enum class Action
{
	IDLE,
	WALK_RIGHT,
	WALK_LEFT,
	CROUCH,
	JUMP,
	UPSTAIR,
	DOWNSTAIR,
	ATTACK,
	USE_ITEM,
	GO_IN,
	GET_DAMAGED,
	DIE,
};

class CMovableObject : public CGameObject
{
protected:

	float dx;
	float dy;

	float vx;
	float vy;

	DWORD dt;
	Action action;

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

	/*
		Simple handling collision: blocking when touch ground (only ground)
		Generically use for falling-down objects.
	*/
	virtual void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents);
	virtual void SetAction(Action action) { this->action = action; };
	virtual void SetState(int state) override;

	CMovableObject();
};

typedef CMovableObject * LPMOVABLEOBJECT;

