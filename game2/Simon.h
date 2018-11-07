#pragma once
#include "MovableObject.h"
#include "Rope.h"

#define SIMON_IDLE_BBOX_WIDTH			32.0f
#define SIMON_IDLE_BBOX_HEIGHT			62.0f
#define SIMON_CROUCHING_BBOX_WIDTH		32.0f
#define SIMON_CROUCHING_BBOX_HEIGHT		46.0f

#define SIMON_WALKING_SPEED				0.12f		// DELETE ME: DONT CHANGE
#define SIMON_JUMP_SPEED_Y				0.35f		// DELETE ME: DONT CHANGE
#define SIMON_JUMP_GRAVITY				0.001f		// DELETE ME: 

#define SIMON_STATE_IDLE				100
#define SIMON_STATE_WALK_RIGHT			101
#define SIMON_STATE_WALK_LEFT			102
#define SIMON_STATE_CROUCH				103
#define SIMON_STATE_JUMP				104
#define SIMON_STATE_UPSTAIR				105
#define SIMON_STATE_DOWNSTAIR			106
#define SIMON_STATE_ATTACK				107
#define SIMON_STATE_GO_IN				111
#define SIMON_STATE_DIE					112

#define ATTACKING_TIME					350
#define FLICKERING_TIME					1000

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
	DWORD flickerStartTime;
	CRope * rope;

	static CSimon * __instance;
	CSimon();

public:

	void SetState(int state) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;
	
	void SetMatchedAnimation(int state);
	void StartToAttack();
	void StartToFlicker();
	void CalibrateCameraPosition();		// To keep Simon at center of camera

	static CSimon * GetInstance();
};

