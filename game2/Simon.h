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
#define SIMON_MAX_SPEED_BY_JUMP_GRAVITY			0.26f		// max fall down speed can get by jump gravity
#define SIMON_MAX_SPEED_Y						1.8f

#define SIMON_DAMAGED_DEFLECT_X					0.1f
#define SIMON_DAMAGED_DEFLECT_Y					-0.4f

#define ATTACK_TIME								350
#define AUTO_CROUCH_TIME						500
#define FREEZING_TIME_TOUCHING_ITEM				1000
#define FLICKERING_TIME							1000
#define UNTOUCHABLE_TIME						1000

#define SIMON_ATTACK_BY_ROPE					0
#define SIMON_ATTACK_BY_ITEM					1

// For flickering while being untouchable.
#define SIMON_UNTOUCHABLE_ALPHA_VALUE			100	


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

	DWORD attack_start;					// If this equals 0, Simon is not attacking
	DWORD auto_crouch_start;			// If this equals 0, Simon is not auto crouching
	
	/*
		If this equals 0, Simon is not being untouchable
		Else if equals -1, Simon is being untouchable, 
		but this won't be use as a counter to stop being untouchable.
	*/
	DWORD untouchable_start;	

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

	void Flicker();
	void StandUp();
	void AutoCrouch();
	void BeUntouchable();
	int OnGetDamaged(LPCOLLISIONEVENT e);

	void MoveRight();
	void MoveLeft();
	void Crouch();
	void Jump();
	void Attack(int choice = SIMON_ATTACK_BY_ROPE);
	void Idle();
	void UseWeapon();

	static CSimon * GetInstance();
};

