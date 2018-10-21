#pragma once
#include "GameObject.h"

#define SIMON_IDLE_BBOX_WIDTH 1111
#define SIMON_IDLE_BBOX_HEIGHT 1112
#define SIMON_CROUCHING_BBOX_WIDTH 1123123
#define SIMON_CROUCHING_BBOX_HEIGHT 213213123

#define SIMON_WALKING_SPEED				0.1f

#define SIMON_JUMP_SPEED_Y				0.5f
#define SIMON_GRAVITY					0.1f

#define SIMON_STATE_IDLE				0
#define SIMON_STATE_WALKING_RIGHT		100
#define SIMON_STATE_WALKING_LEFT		101
#define SIMON_STATE_CROUCHING			200
#define SIMON_STATE_JUMPING				300
#define SIMON_STATE_ATTACKING			900

#define ATTACKING_TIME					300

enum SimonAniID
{
	IDLE_RIGHT = 9900,
	IDLE_LEFT,
	WALK_RIGHT,
	WALK_LEFT,
	CROUCH_RIGHT,
	CROUCH_LEFT,
	ATTACK_RIGHT,
	ATTACK_LEFT
};

class CSimon : public CGameObject
{
	int attacking;
	bool flag = true;
	DWORD attacking_start_time;

public:
	CSimon();
	~CSimon();
	
	void SetState(int state);
	void StartToAttack();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

