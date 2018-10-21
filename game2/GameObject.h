#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Animations.h"

using namespace std;

#define ID_TEX_BBOX -100


class CGameObject;
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT &a, const LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
protected:

	float x;
	float y;

	int state;

	DWORD dt;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float &x, float &y) { x = this->x, y = this->y; }

	int GetState() { return this->state; }
	void RenderBoundingBox();
	
	virtual void SetState(int state) { this->state = state; }
	virtual void AddAnimation(int aniID) = 0;
	virtual void Render() = 0;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;

	CGameObject();
};

