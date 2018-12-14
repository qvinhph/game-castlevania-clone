#pragma once
#include "MovableObject.h"

#define ZOMBIE_WALKING_SPEED	0.1f	
#define ZOMBIE_BBOX_HEIGHT		64.0f
#define ZOMBIE_BBOX_WIDTH		32.0f
#define ZOMBIE_DEFAULT_HEALTH	1
#define ZOMBIE_POINT_AWARD		100
#define ZOMBIE_DAMAGE			2

enum class ZombieAniID
{
	WALK_LEFT = 5100,
	WALK_RIGHT,
};

class CZombie : public CActiveObject
{

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) override;
	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;
	void SetState(int state) override;

	CZombie();
};

