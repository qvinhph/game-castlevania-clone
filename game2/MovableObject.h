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
	UPSTAIRS,
	DOWNSTAIRS,
	ATTACK,
	USE_ITEM,
	GO_IN,
	GET_DAMAGED,
	DIE,
};


/*
	The object that need be updated every frame.
*/
class CActiveObject : public CGameObject
{
protected:

	float dx;
	float dy;

	float vx;
	float vy;

	DWORD dt;

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
	virtual void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents);
	virtual void SetState(int state) override;

	CActiveObject();
};

typedef CActiveObject * LPACTIVEOBJECT;

