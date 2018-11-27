#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Animations.h"

using namespace std;

#define ID_TEX_BBOX			-100

#define STATE_VISIBLE		0
#define STATE_INVISIBLE		-1




// Item or dropable objects' name
enum class Item
{
	NONE,
	ITEMROPE,
	HEART,
	BIGHEART,
	ITEMDAGGER,
};

// Weapon name used for second attacking 
enum class Weapon
{
	NONE,
	DAGGER,
};

// Monsters' name
enum class Monsters
{
	ZOMBIE,
};


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
	int nx;				// direction of the object: > 0 will be face to right-side, otherwise left-side

	CAnimations* animations;		// pointer helping getting the animations
	int currentAniID;
	int lastAniID = -1;

	ARGB argb;						// for changing color of the sprite
	Item item;						// the item object holds, drop when destroyed

	bool freezing; 
	DWORD flicker_start;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float &x, float &y) { x = this->x, y = this->y; }

	void SetDirection(int nx) { this->nx = nx; }
	int GetDirection() { return this->nx; }

	void SetHoldingItem(Item item) { this->item = item; }
	Item GetHoldingItem() { return this->item; }				// return the item name (enum class)

	virtual void SetFreezing(bool freezing) { this->freezing = freezing; }
	bool GetFreezing() { return this->freezing; }

	virtual void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }
	
	void RenderBoundingBox();
	void FreezeAnimation();
	bool IsInViewport();
	void ReDirect(float posisionX);
	void ResetAnimationTimer(int aniID);

	virtual bool IsOverlapping(LPGAMEOBJECT obj);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Render();
	virtual void Destroy();		

	CGameObject();
};

