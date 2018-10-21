#pragma once
#include "GameObject.h"

#define SIMON_IDLE_BBOX_WIDTH			1111
#define SIMON_IDLE_BBOX_HEIGHT			1112
#define SIMON_CROUCHING_BBOX_WIDTH		1123123
#define SIMON_CROUCHING_BBOX_HEIGHT		213213123

#define SIMON_WALKING_SPEED				0.1f
#define SIMON_JUMP_SPEED_Y				0.5f
#define SIMON_GRAVITY					0.002f

#define SIMON_STATE_IDLE				0
#define SIMON_STATE_WALK_RIGHT			100
#define SIMON_STATE_WALK_LEFT			101
#define SIMON_STATE_CROUCH				200
#define SIMON_STATE_JUMP				300
#define SIMON_STATE_UPSTAIR				400
#define SIMON_STATE_DOWNSTAIR			410
#define SIMON_STATE_ATTACK				900
#define SIMON_STATE_CROUCH_ATTACK		910
#define SIMON_STATE_UPSTAIR_ATTACK		910
#define SIMON_STATE_DOWNSTAIR_ATTACK	920
#define SIMON_STATE_GO_IN				999
#define SIMON_STATE_DIE					1000

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
	ATTACK_LEFT,
	PUSHED_BACK_RIGHT,
	PUSHED_BACK_LEFT,
	DOWN_STAIR_RIGHT,
	DOWN_STAIR_LEFT,
	UP_STAIR_RIGHT,
	UP_STAIR_LEFT,
	CROUCH_ATTACK_RIGHT,
	CROUCH_ATTACK_LEFT,
	DOWN_STAIR_ATTACK_RIGHT,
	DOWN_STAIR_ATTACK_LEFT,
	UP_STAIR_ATTACK_RIGHT,
	UP_STAIR_ATTACK_LEFT,
	GO_IN
};

class CSimon : public CGameObject
{
	int attacking;
	bool flag = true;
	DWORD attacking_start_time;

public:
	CSimon();
	
	virtual void SetState(int state);
	void StartToAttack();

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void Render();
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

