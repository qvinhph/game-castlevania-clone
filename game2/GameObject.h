#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Animations.h"

using namespace std;

#define ID_TEX_BBOX			-100

#define STATE_VISIBLE		0
#define STATE_INVISIBLE		-1



// The action with this timer is not working.
#define TIMER_IDLE			0

// The action with this timer is not going to end.
#define TIMER_ETERNAL		-1


// Item or dropable objects' name
enum class Item
{
	NONE,
	ITEMROPE,
	HEART,
	BIGHEART,
	ITEMDAGGER,
	ITEMHOLYWATER,
	ITEMAXE,
	ITEMMEAT,
	MONEY_BAG_RED,
	MONEY_BAG_WHITE,
	MONEY_BAG_BLUE,
	CROSS,
	DOUBLE_SHOT,
};

/* 
	Weapon name with the according number is the cost when use it
*/
enum class Weapon
{
	NONE,
	DAGGER,
	AXE,
	HOLYWATER,

	FIREBALL, // of CFish
};

// Monsters' name
enum class Monster
{
	ZOMBIE,
	PANTHER,
	PINKBAT,
	FISH,
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

	static CCamera * cameraInstance;

	float x;
	float y;
	int state;
	int nx;				// direction of the object: > 0 will be face to right-side, otherwise left-side

	CAnimations* animations;		// pointer helping getting the animations
	int currentAniID;
	int lastAniID;

	ARGB argb;						// for changing color of the sprite
	Item item;						// the item object holds, drop when destroyed

	bool freezing; 
	DWORD flicker_start;

	int point = 0;					// the point awarded by this object
	int health = 0;					// the health of the object
	int damage = 0;					// the damage that the object is able to cause

public:
	int GetPoint() { return this->point; }
	int GetHealth() { return this->health; }
	int GetDamage() { return this->damage; }

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

	void SetCurrentAniID(int aniID) { this->currentAniID = aniID; }
	int GetCurrentAniID() { return this->currentAniID; }
	
	void RenderBoundingBox();
	void FreezeAnimation();
	void ReDirect(float posisionX);
	void ResetAnimationTimer(int aniID);
	bool IsInViewport();


	virtual bool IsOverlapping(LPGAMEOBJECT obj);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Render();
	virtual void Destroy();		
	virtual void BeHit(int damage);

	CGameObject();
};

