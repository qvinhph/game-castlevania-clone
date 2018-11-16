#pragma once
#include "MovableObject.h"
#include "Rope.h"
#include "Weapons.h"

#define SIMON_IDLE_BBOX_WIDTH					32.0f
#define SIMON_IDLE_BBOX_HEIGHT					62.0f
#define SIMON_CROUCHING_BBOX_WIDTH				32.0f
#define SIMON_CROUCHING_BBOX_HEIGHT				46.0f
#define SIMON_JUMPING_BBOX_WIDTH				32.0f
#define SIMON_JUMPING_BBOX_HEIGHT				54.0f

#define SIMON_WALKING_SPEED						0.12f		
#define SIMON_JUMP_SPEED						0.4f
#define SIMON_FALL_GRAVITY						0.01f
#define SIMON_JUMP_GRAVITY						0.0012f
#define SIMON_MAX_SPEED_BY_JUMP_GRAVITY			0.26f		// from crouching to standing
#define SIMON_MAX_SPEED_Y						1.5f


#define SIMON_DAMAGED_DEFLECT_X					0.1f
#define SIMON_DAMAGED_DEFLECT_Y					0.4f

#define ATTACKING_TIME							350
#define FLICKERING_TIME							1000
#define AUTO_CROUCH_TIME						500
#define FREEZING_TIME_TOUCHING_ITEM				1000


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
	DIE, 
	DAMAGING_LEFT,
	DAMAGING_RIGHT
};

class CSimon : public CMovableObject
{
	bool controllable;

	bool jumping;
	bool crouching;
	bool stairing;

	DWORD attackStartTime;
	DWORD flickerStartTime;
	DWORD autoCrouchStartTime;
	Weapon secondWeapon;

	CRope * rope;
	CWeapons * weapons;

	static CSimon * __instance;
	CSimon();

public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;
	void SetAction(Action action) override;
	void SetFreezing(bool freezing) override;
	
	void SetMatchedAnimation();
	void CalibrateCameraPosition();		// To keep Simon at center of camera

	void StartToAttack(Weapon secondWeapon = Weapon::NONE);
	void StartToFlicker();
	
	void StandUp();
	void AutoCrouch();

	static CSimon * GetInstance();
};

