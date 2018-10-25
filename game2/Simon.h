#pragma once
#include "MovableObject.h"
#include "Rope.h"

#define SIMON_IDLE_BBOX_WIDTH			16
#define SIMON_IDLE_BBOX_HEIGHT			30
#define SIMON_CROUCHING_BBOX_WIDTH		16
#define SIMON_CROUCHING_BBOX_HEIGHT		23

#define SIMON_ATTACKING_BBOX_WIDTH		16

#define SIMON_WALKING_SPEED				0.1f
#define SIMON_JUMP_SPEED_Y				0.3f
#define SIMON_GRAVITY					0.001f

#define SIMON_STATE_IDLE				100
#define SIMON_STATE_WALK_RIGHT			101
#define SIMON_STATE_WALK_LEFT			102
#define SIMON_STATE_CROUCH				103
#define SIMON_STATE_JUMP				104
#define SIMON_STATE_UPSTAIR				105
#define SIMON_STATE_DOWNSTAIR			106
#define SIMON_STATE_ATTACK				107
#define SIMON_STATE_CROUCH_ATTACK		108
#define SIMON_STATE_UPSTAIR_ATTACK		109
#define SIMON_STATE_DOWNSTAIR_ATTACK	110
#define SIMON_STATE_GO_IN				111
#define SIMON_STATE_DIE					112

#define ATTACKING_TIME					1250//350 

enum class SimonAniID
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
	GO_IN,
	DIE
};

class CSimon : public CMovableObject
{
	bool attacking;
	bool jumping;
	bool crouching;
	bool stairing;
	DWORD attackStartTime;
	CRope * rope;

	static CSimon * __instance;
	CSimon();

public:
	
	void StartToAttack();

	void SetState(int state) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

	void ProcessCollision(vector<LPCOLLISIONEVENT> &coEvents);

	static CSimon * GetInstance();
};

