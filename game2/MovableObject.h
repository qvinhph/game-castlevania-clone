#pragma once
#include "GameObject.h"

#define WORLD_FALL_SPEED	0.18f

class CMovableObject : public CGameObject
{
protected:

	float dx;
	float dy;

	float vx;
	float vy;

	int nx;	
	DWORD dt;

public:

	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx, vy = this->vy; }

	void SetDirection(int nx) { this->nx = nx; }
	void GetDirection(int &nx) { nx = this->nx; }

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

	CMovableObject();
};

typedef CMovableObject * LPMOVABLEOBJECT;

