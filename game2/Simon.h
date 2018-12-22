#pragma once
#include "MovableObject.h"
#include "Rope.h"
#include "Weapons.h"


#define SIMON_IDLE_BBOX_WIDTH			32.0f
#define SIMON_IDLE_BBOX_HEIGHT			62.0f
#define SIMON_CROUCHING_BBOX_WIDTH		32.0f
#define SIMON_CROUCHING_BBOX_HEIGHT		46.0f
#define SIMON_JUMPING_BBOX_WIDTH		32.0f
#define SIMON_JUMPING_BBOX_HEIGHT		54.0f

#define SIMON_STAIR_SPEED_X						0.051f
#define SIMON_STAIR_SPEED_Y						0.051f
#define SIMON_WALKING_SPEED						0.12f		
#define SIMON_JUMP_SPEED						0.4f
#define SIMON_JUMP_GRAVITY						0.0012f
#define SIMON_FALL_GRAVITY						0.012f 
#define SIMON_MAX_SPEED_WITH_JUMP_GRAVITY		0.26f		// max fall down speed can get by jump gravity
#define SIMON_MAX_SPEED_Y						1.8f
#define SIMON_AUTO_MOVE_SPEED_X					0.06f

#define SIMON_DAMAGED_DEFLECT_X					0.1f
#define SIMON_DAMAGED_DEFLECT_Y					-0.4f

#define SIMON_ATTACK_TIME						350
#define SIMON_AUTO_CROUCH_TIME					300
#define SIMON_TOUCH_ITEM_FREEZING_TIME			1000
#define SIMON_FLICKERING_TIME					1000
#define SIMON_UNTOUCHABLE_TIME					1000
#define SIMON_AUTO_STAIR_TIME					300
#define SIMON_AUTO_MOVE_TIME					1200

#define SIMON_ATTACK_BY_ROPE					0
#define SIMON_ATTACK_BY_ITEM					1

// For flickering while being untouchable.
#define SIMON_UNTOUCHABLE_ALPHA_VALUE			100	
#define SCREEN_FLASH_TIME						600

#define COLOR_WHITE(alpha)			D3DCOLOR_XRGB(alpha, 255, 255, 255)


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
	WALK_DOWNSTAIRS_RIGHT,
	WALK_DOWNSTAIRS_LEFT,
	WALK_UPSTAIRS_RIGHT,
	WALK_UPSTAIRS_LEFT,
	IDLE_DOWNSTAIRS_RIGHT,
	IDLE_DOWNSTAIRS_LEFT,
	IDLE_UPSTAIRS_RIGHT,
	IDLE_UPSTAIRS_LEFT,
	CROUCH_ATTACK_RIGHT,
	CROUCH_ATTACK_LEFT,
	DOWN_STAIR_ATTACK_RIGHT,
	DOWN_STAIR_ATTACK_LEFT,
	UP_STAIR_ATTACK_RIGHT,
	UP_STAIR_ATTACK_LEFT,
	GO_IN,
	DIE_LEFT, 
	DIE_RIGHT, 
	DAMAGING_LEFT,
	DAMAGING_RIGHT
};

struct AutoInfo
{
	float vx;
	float vy;
	float xDes;
	float yDes;
	DWORD autoTimeLast;
};

class CSimon : public CActiveObject
{
	// Flags
	bool controllable; 
	bool autoMove;	   
	bool jumping;	   
	bool crouching;	   
	bool dying = false;

	// Assigned to -1 if going downstairs
	// Assigned to 1 if going upstairs
	// Assigned to 0 if not on stairs
	int onStairs;

	// Timers, also be used as FLAG for the action when assigned to TIMER_IDLE(false) or non-TIMER_IDLE(true)
	// Assigned to TIMER_IDLE			: the timer is not working,
	// Assigned to TIMER_ETERNAL		: the timer is working and the action is not going to stop
	// Assigned to unsigned number		: the timer is working
	DWORD attack_start;					
	DWORD auto_crouch_start;			
	DWORD auto_start;

	// To make the flashing screen effect
	DWORD flash_start;
	bool whiteBackground = false;

	int numberOfHearts;
	AutoInfo autoInfo;
	Weapon secondaryWeapon;
	CRope * rope;
	CWeapons * weaponsInstance;
	vector<LPGAMEOBJECT> ovObjects;		// overlapping objects
	vector<LPGAMEOBJECT>* coObjects;	// for saving the current coObjects to manipulate easily

	static CSimon * __instance;
	CSimon();

public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;
	void SetFreezing(bool freezing) override;
	void ProceedBeingUntouchable() override;
	
	void PickAnimation();
	void CalibrateCameraPosition();		// To keep Simon at center of camera and not go outside
	void SetAction(Action action);

	void Flicker();
	void StandUp();
	void AutoCrouch();
	void BeUntouchable();
	void OnGetDamaged(LPCOLLISIONEVENT e);
	void ProceedAutoMove();
	void ProceedFlickering();
	void ProceedAttacking();
	void ProceedAutoCrouching();
	void ProceedJumping();
	void ProceedGravity();
	void ProceedOnStairs();
	void ProceedOverlapping();
	void ProceedFlashingScreen();
	void StartAutoMove(float vx, float xDestination);
	void StartAutoMove(float vx, float vy, DWORD time);
	bool IsAbleToUseWeapon();
	void KillAllMonsters();

	void MoveRight();
	void MoveLeft();
	void Crouch();
	void Jump();
	void Attack(int choice = SIMON_ATTACK_BY_ROPE);
	void Idle();
	void UseWeapon();
	void Upstairs();
	void Downstairs();

	Weapon GetSecondaryWeapon() { return this->secondaryWeapon; }

	// DEBUGGING: Delete me
	void RemoveOnStairs() {
		this->onStairs = false;
	}

	static CSimon * GetInstance();
};

