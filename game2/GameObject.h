#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Animations.h"

using namespace std;

#define ID_TEX_BBOX			-100

#define INSIVIBLE_ZONE_X	-99999
#define INSIVIBLE_ZONE_Y	-99999

#define STATE_VISIBLE		0
#define STATE_INVISIBLE		-1

enum class Item
{
	NONE,
	ITEMROPE,
	HEART,
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

	CAnimations* animations;		// pointer helping getting the animations
	int currentAniID;

	bool flickering;				// use for flickerable objects
	ARGB argb;
	Item item;						// the item object holds, drop when destroyed

public:

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float &x, float &y) { x = this->x, y = this->y; }

	void SetHoldingItem(Item item) { this->item = item; }
	Item GetHoldingItem() { return this->item; }				// return the item name (enum class)

	virtual void SetState(int state);
	int GetState() { return this->state; }
	
	void RenderBoundingBox();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	
	virtual void ResetAnimation(int aniID);	
	virtual void Render();
	virtual void Destroy();			// Add destroying effect and send object to invisible zone

	CGameObject();
};

