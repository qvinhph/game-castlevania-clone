#include <algorithm>

#include "Simon.h"
#include "Game.h"
#include "Animations.h"
#include "BigCandle.h"
#include "ItemRope.h"
#include "BigHeart.h"
#include "InvisibleWall.h"
#include "Heart.h"
#include "ItemDagger.h"
#include "Flame.h"
#include "Dagger.h"
#include "Timer.h"
#include "Zombie.h"
#include "Candle.h"
#include "StairsUp.h"
#include "StairsDown.h"
#include "Panther.h"
#include "PinkBat.h"

#include "debug.h"

CSimon * CSimon::__instance = NULL;

CSimon::CSimon()
{
	this->controllable = true;
	this->jumping = false;
	this->crouching = false;
	this->autoMove = false;
	this->secondWeapon = Weapon::NONE;

	this->auto_crouch_start = 0;
	this->attack_start = 0;
	this->flicker_start = 0;
	this->auto_start = 0;
	this->onStairs = 0;

	// ready to be used
	rope = CRope::GetInstance();
	weapons = CWeapons::GetInstance();
}

void CSimon::Render()
{	
	if (flicker_start != 0)
	{
		// Flicker by getting damaged.
		if (untouchable_start != 0)
		{
			if (this->argb.alpha != SIMON_UNTOUCHABLE_ALPHA_VALUE)
				this->argb.alpha = SIMON_UNTOUCHABLE_ALPHA_VALUE;
			else 
				this->argb = ARGB();
		}

		// Flicker by touching item-rope.
		else
		{
		// Animation frame will be rendered with
		// these 3 colors defined by change the argb value
			if (this->argb.blue != 0)
				this->argb.blue = 0;

			else if (argb.green != 0)
				this->argb.green = 0;

			else
				this->argb = ARGB();
		}		
	}
	
	CGameObject::Render();
}

void CSimon::PickAnimation()
{
	if (attack_start != 0)
	{
		if (crouching)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::CROUCH_ATTACK_RIGHT :
				(int)SimonAniID::CROUCH_ATTACK_LEFT;

		else if (onStairs == 1)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::UP_STAIR_ATTACK_RIGHT :
				(int)SimonAniID::UP_STAIR_ATTACK_LEFT;

		else if (onStairs == -1)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::DOWN_STAIR_ATTACK_RIGHT :
				(int)SimonAniID::DOWN_STAIR_ATTACK_LEFT;

		else
			currentAniID = (nx > 0) ?
				(int)SimonAniID::ATTACK_RIGHT :
				(int)SimonAniID::ATTACK_LEFT;
	}

	else if (onStairs == 1)
	{
		if (vx != 0)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::WALK_UPSTAIRS_RIGHT :
				(int)SimonAniID::WALK_UPSTAIRS_LEFT;

		else
			currentAniID = (nx > 0) ?
				(int)SimonAniID::IDLE_UPSTAIRS_RIGHT :
				(int)SimonAniID::IDLE_UPSTAIRS_LEFT;

	}

	else if (onStairs == -1)
	{
		if (vx != 0)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::WALK_DOWNSTAIRS_RIGHT :
				(int)SimonAniID::WALK_DOWNSTAIRS_LEFT;
		else
			currentAniID = (nx > 0) ?
				(int)SimonAniID::IDLE_DOWNSTAIRS_RIGHT :
				(int)SimonAniID::IDLE_DOWNSTAIRS_LEFT;
	}

	else if (untouchable_start == -1)
		currentAniID = (nx > 0) ?
		(int)SimonAniID::DAMAGING_RIGHT :
		(int)SimonAniID::DAMAGING_LEFT;

	// these two action use the same animation
	else if (crouching || jumping)
	{
		currentAniID = (nx > 0) ?
			(int)SimonAniID::CROUCH_RIGHT :
			(int)SimonAniID::CROUCH_LEFT;

		if (jumping && !crouching)
			currentAniID = (nx > 0) ?
			(int)SimonAniID::IDLE_RIGHT :
			(int)SimonAniID::IDLE_LEFT;
	}

	else if (vx > 0)
	{
		currentAniID = (int)SimonAniID::WALK_RIGHT;
	}

	else if (vx < 0)
	{
		currentAniID = (int)SimonAniID::WALK_LEFT;
	}

	else if (vx == 0)
	{
		currentAniID = (nx > 0) ?
			(int)SimonAniID::IDLE_RIGHT :
			(int)SimonAniID::IDLE_LEFT;
	}
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy
	CActiveObject::Update(dt);
	

	// While jumping and falling
	if (jumping)
		ProceedJumping();
	else
		if (onStairs == 0)
			ProceedGravity();


	// Keep vy not to be too high
	if (vy > SIMON_MAX_SPEED_Y)
		vy = SIMON_MAX_SPEED_Y;


	// Auto Crouch
	if (auto_crouch_start != TIMER_IDLE)
		ProceedAutoCrouching();


	// Attacking
	if (attack_start != TIMER_IDLE)
		ProceedAttacking();


	// Being Untouchable
	if (untouchable_start != TIMER_IDLE &&
		untouchable_start != TIMER_ETERNAL)
		ProceedBeingUntouchable();


	// Flickering
	if (flicker_start != TIMER_IDLE)
		ProceedFlickering();


	// Auto Move: this will re-calculate the dx, dy
	if (autoMove)
		ProceedAutoMove();


	// Being On Stairs
	if (onStairs != 0)
		ProceedOnStairs();


	PickAnimation();


	// Overlapping
	ovObjects.clear();
	for (UINT i = 0; i < coObjects->size(); ++i)
		if (this->IsOverlapping(coObjects->at(i)))
			ovObjects.push_back(coObjects->at(i));


	// Collisions
	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();

	if (true) // TO-DO: if not dead
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
		ProceedCollisions(coEvents);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	

	CalibrateCameraPosition();
}

void CSimon::ProceedFlickering()
{
	if (GetTickCount() - flicker_start >= FLICKERING_TIME)
	{
		this->argb = ARGB();
		flicker_start = TIMER_IDLE;
	}
}

void CSimon::ProceedBeingUntouchable()
{
	if (GetTickCount() - untouchable_start > UNTOUCHABLE_TIME)
	{
		// Stop flickering effect
		this->argb = ARGB();
		flicker_start = TIMER_IDLE;

		untouchable_start = TIMER_IDLE;
	}
}

void CSimon::ProceedAttacking()
{
	if (GetTickCount() - attack_start > ATTACK_TIME)
	{
		// If attacking by rope
		if (this->rope->GetState() == STATE_VISIBLE) 
			this->rope->SetState(STATE_INVISIBLE);
		else
			weapons->UseWeapon(secondWeapon, this);

		// Stop timer
		attack_start = TIMER_IDLE;
	}
	else 
		// While attacking, Simon can't move horizontally, except finish jumping
		if (!jumping)
			vx = 0;
}

void CSimon::ProceedAutoCrouching()
{
	if (GetTickCount() - auto_crouch_start >= AUTO_CROUCH_TIME)
	{
		this->StandUp();

		// Stop counting
		auto_crouch_start = TIMER_IDLE;
	}
}

void CSimon::ProceedJumping()
{
	// When reach this speed, Simon will stand up and fall faster.
	if (vy > SIMON_MAX_SPEED_WITH_JUMP_GRAVITY)
	{
		vy += SIMON_FALL_GRAVITY * this->dt;

		if (crouching == true)
			this->StandUp();

	}
	else
		vy += SIMON_JUMP_GRAVITY * this->dt;
}

void CSimon::ProceedGravity()
{
	// If falling
	if (vy != 0)
		controllable = false;

	vy += SIMON_FALL_GRAVITY * this->dt;
}

/*
	Help get out of the stairs
*/
void CSimon::ProceedOnStairs()
{
	// Try getting out the stairs
	LPGAMEOBJECT stairs = NULL;
	
	if (onStairs == 1)
	{
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairsDown *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}

		if (stairs == NULL) return;

		float xS, yS;
		stairs->GetPosition(xS, yS);

		// Has reached the stairs exit
		if (y < yS)
		{
			y = yS - DEFLECTION_AVOID_OVERLAPPING;
			onStairs = 0;

			autoMove = false;
			vx = vy = 0;
		}
	}
	else if (onStairs == -1)
	{
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairsUp *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}

		if (stairs == NULL) return;

		float xS, yS;
		stairs->GetPosition(xS, yS);

		// Has reached the stairs exit
		if (y > yS)
		{
			y = yS - DEFLECTION_AVOID_OVERLAPPING;
			onStairs = 0;

			autoMove = false;
			vx = vy = 0;
		}
	}
}

void CSimon::ProceedAutoMove()
{
	// Auto move and stop when reach destination
	if (auto_start == TIMER_IDLE)
	{
		if (autoInfo.vx != 0 &&
			autoInfo.xDes != x)
		{
			ReDirect(autoInfo.xDes);
			vx = nx * autoInfo.vx;
			dx = vx * dt;

			// Calibrate the dx if object is moving far away from the destination
			if (dx > 0)
			{
				if (x + dx > autoInfo.xDes)
					dx = autoInfo.xDes - x;
			}
			else
			{
				if (x + dx < autoInfo.xDes)
					dx = autoInfo.xDes - x;
			}
		}
		else
		{
			autoMove = false;
			controllable = true;
			dx = dy = vx = vy = 0;
		}
	}

	// Auto move and stop when reach the time
	else
	{
		if (GetTickCount() - auto_start > autoInfo.autoTimeLast)
		{
			auto_start = TIMER_IDLE;
			autoMove = false;
			controllable = true;
			dx = dy = vx = vy = 0;
		}
		else
		{
			vx = autoInfo.vx;
			vy = autoInfo.vy;
			dx = vx * dt;
			dy = vy * dt;
		}
	}
}

void CSimon::ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;
	float min_tx, min_ty, nx, ny;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	// Set the object's position right to the point occured collision
	x += min_tx * dx;
	y += min_ty * dy;

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		if (dynamic_cast<CBigCandle *>(e->obj) ||
			dynamic_cast<CFlame *>(e->obj) ||
			dynamic_cast<CDagger *>(e->obj) ||
			dynamic_cast<CCandle *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch something but do nothing !!");

			// Ignore other collisions by completing the rest of dx / dy without blocked.
			if (e->nx != 0)	x += (1 - min_tx) * dx;
			if (e->ny != 0)	y += (1 - min_ty) * dy;
		}

		else if (dynamic_cast<CBigHeart *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Big Heart");

			e->obj->SetState(STATE_INVISIBLE);
		}

		else if (dynamic_cast<CItemRope *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Item Rope");

			e->obj->SetState(STATE_INVISIBLE);
			this->rope->LevelUp();
			this->Flicker();
			CTimer::GetInstance()->Freeze(FREEZING_TIME_TOUCHING_ITEM);
		}

		else if (dynamic_cast<CHeart *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Heart");

			e->obj->SetState(STATE_INVISIBLE);
		}

		else if (dynamic_cast<CItemDagger *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Dagger");

			e->obj->SetState(STATE_INVISIBLE);
			secondWeapon = Weapon::DAGGER;
			weapons->AddToStock(Weapon::DAGGER);
		}

		else if (dynamic_cast<CZombie *>(e->obj) ||
				dynamic_cast<CPanther *>(e->obj) ||
				dynamic_cast<CPinkBat *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch a monster !!");

			if (untouchable_start == TIMER_IDLE)
			{
				this->OnGetDamaged(e);
				return;
			}
			
			else
			{
				// Ignore the collision by completing the rest of dx/dy
				if (e->nx != 0) x += (1 - min_tx) * dx;
				if (e->ny != 0) y += (1 - min_ty) * dy;
			}
		}

		// block with ground objects
		else if (dynamic_cast<CInvisibleWall *>(e->obj))
		{
			// Simon ignores the ground while on stairs
			if (onStairs != 0)
				continue;

			if (e->nx != 0)
			{
				// Prevent overlapping next frame
				x += nx * DEFLECTION_AVOID_OVERLAPPING;
			}

			if (e->ny < 0)
			{
				// Regain the control
				// In case Simon is not "auto move"
				if (!autoMove)
					controllable = true;


				if (jumping)	
				{
					this->StandUp();
					jumping = false;
				}


				// When falls from too high place or gets damaged
				if (vy == SIMON_MAX_SPEED_Y || 
					untouchable_start == TIMER_ETERNAL)
				{
					AutoCrouch();

					if (untouchable_start == TIMER_ETERNAL)
						this->BeUntouchable();
				}


				// Prevent overlapping next frame
				y += ny * DEFLECTION_AVOID_OVERLAPPING;
				vy = 0;
			}
		}
	}
}

void CSimon::Flicker()
{
	if (flicker_start == TIMER_IDLE)
		flicker_start = GetTickCount();
}

/*
	The function helps Simon stand up without overlapping 
	objects by re-locating Simon position
*/
void CSimon::StandUp()
{
	if (jumping && crouching)
	{
		crouching = false;
		y += SIMON_JUMPING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
	}
	else if (crouching && attack_start == TIMER_IDLE)
	{
		crouching = false;
		y += SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
	}

		
}

void CSimon::AutoCrouch()
{
	SetAction(Action::CROUCH);

	// If setting action succeeded
	if (crouching)
		auto_crouch_start = GetTickCount();
}

/*
	Use this to start the untouchable_start timer
*/
void CSimon::BeUntouchable()
{
	if (untouchable_start == TIMER_IDLE ||
		untouchable_start == TIMER_ETERNAL)
	{
		untouchable_start = GetTickCount();
		this->Flicker();
	}
}

void CSimon::OnGetDamaged(LPCOLLISIONEVENT e)
{
	if (onStairs == 0)
	{
		this->nx = (e->nx != 0) ?
			-(e->nx) :
			-(e->obj->GetDirection());

		vx = vy = dx = dy = 0;

		this->vx = (-this->nx) * SIMON_DAMAGED_DEFLECT_X;
		this->vy = SIMON_DAMAGED_DEFLECT_Y;

		// Set jumping = true to re-use the SIMON_JUMP_GRAVITY
		jumping = true;
		controllable = false;

		// Simon is being untouchable but not going to stop being untouchable
		untouchable_start = TIMER_ETERNAL;
	}
	else
	{
		untouchable_start = GetTickCount();
		flicker_start = GetTickCount();
	}	
}

void CSimon::MoveRight()
{
	if (onStairs == 0)
	{
		if (attack_start == 0 &&	// While attacking, Simon cannot move
			!jumping)				// While jumping, Simon cannot move
		{
			this->nx = 1;
			if (!crouching)
				this->vx = SIMON_WALKING_SPEED * this->nx;
		}
	}	
	else
	{
		if (onStairs == 1)
			if (nx == 1)
				Upstairs();
			else
				Downstairs();

		else if (onStairs == -1)
			if (nx == 1)
				Downstairs();
			else
				Upstairs();
	}
}

void CSimon::MoveLeft()
{
	if (onStairs == 0)
	{
		if (attack_start == TIMER_IDLE &&	// While attacking, Simon cannot move
			!jumping)						// While jumping, Simon cannot move
		{
			this->nx = -1;

			// Can only change direction while crouching
			if (!crouching)
				this->vx = SIMON_WALKING_SPEED * this->nx;
		}
	}
	else
	{
		if (onStairs == 1)
			if (nx == 1)
				Downstairs();
			else
				Upstairs();

		else if (onStairs == -1)
			if (nx == 1)
				Upstairs();
			else
				Downstairs();
	}	
}

void CSimon::Crouch()
{
	if (!crouching && 
		onStairs == 0 &&
		attack_start == 0)
	{
		this->crouching = true;
		this->vx = 0;

		// Re-locate Simon to avoid overlapping
		this->y += SIMON_IDLE_BBOX_HEIGHT - SIMON_CROUCHING_BBOX_HEIGHT;
	}	
}

void CSimon::Idle()
{
	if (!jumping)
	{
		this->vx = 0;

		if (crouching && auto_crouch_start == TIMER_IDLE)
			this->StandUp();
	}
}

/*
	Use the weapon or item, if has
*/
void CSimon::UseWeapon()
{
	if (secondWeapon == Weapon::NONE)
		this->Attack(SIMON_ATTACK_BY_ROPE);

	else if (secondWeapon == Weapon::DAGGER)
		this->Attack(SIMON_ATTACK_BY_ITEM);
}

void CSimon::Upstairs()
{
	// Check if Simon is on stairs
	// If not, try to get into the stair
	if (onStairs == 0)
	{
		LPGAMEOBJECT stairs = NULL;

		// Check if Simon is on a stairs-start
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairsUp *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}

		// Do nothing if there is no stairs overlapped
		if (stairs == NULL) return;

		// Try to reach the right point of stairs
		float xS, yS;
		stairs->GetPosition(xS, yS);
		if (x != xS)
			StartAutoMove(SIMON_WALKING_SPEED, xS);
		else
		{
			nx = stairs->GetDirection();
			StartAutoMove(nx * SIMON_STAIR_SPEED_X, -SIMON_STAIR_SPEED_Y, SIMON_AUTO_STAIR_TIME);
			onStairs = 1;
		}
	}
	
	else
	{
		// Simon is going downstairs and wanting to climb back
		if (onStairs == -1)
		{
			this->nx = -nx;
			onStairs = 1;
		}

		StartAutoMove(nx * SIMON_STAIR_SPEED_X, -SIMON_STAIR_SPEED_Y, SIMON_AUTO_STAIR_TIME);
	}
}

void CSimon::Downstairs()
{
	// Check if Simon is onStairs
	// If not, try to get into the stairs
	if (onStairs == 0)
	{
		// Check if Simon is on a stairs-start
		LPGAMEOBJECT stairs = NULL;
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairsDown *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}

		// Do nothing if there is no stairs overlapped
		if (stairs == NULL) return;

		// Try to reach the right point of stairs
		float xS, yS;
		stairs->GetPosition(xS, yS);
		if (x != xS)
			StartAutoMove(SIMON_WALKING_SPEED, xS);
		else
		{
			nx = stairs->GetDirection();
			StartAutoMove(nx * SIMON_STAIR_SPEED_X, SIMON_STAIR_SPEED_Y, SIMON_AUTO_STAIR_TIME);
			onStairs = -1;
		}
	}

	else
	{
		// Simon is going upstairs and wanting to go back
		if (onStairs == 1)
		{
			nx = -nx;
			onStairs = -1;
		}

		StartAutoMove(nx * SIMON_STAIR_SPEED_X, SIMON_STAIR_SPEED_Y, SIMON_AUTO_STAIR_TIME);
	}

}

/*
	Move automatically to the given position (x-axis) with the given speed.
	Move horizontally only.
*/
void CSimon::StartAutoMove(float vx, float xDestination)
{
	if (!autoMove)
	{
		autoInfo.xDes = xDestination;
		autoInfo.vx = vx;

		autoMove = true;
		controllable = false;
	}
}

void CSimon::StartAutoMove(float vx, float vy, DWORD time)
{
	// Can only perform one auto-move at a time
	if (!autoMove)
	{
		autoInfo.vx = vx;
		autoInfo.vy = vy;
		autoInfo.autoTimeLast = time;

		autoMove = true;
		auto_start = GetTickCount();
		controllable = false;
	}
}

void CSimon::Jump()
{
	if (!jumping && !crouching
		&& attack_start == TIMER_IDLE 
		&& onStairs == 0)
	{
		this->jumping = true;
		this->crouching = true;
		this->vy = -SIMON_JUMP_SPEED;		
	}
}

void CSimon::Attack(int choice)
{
	if (attack_start == 0)
	{
		attack_start = GetTickCount();

		// Simon behaviors
		if (jumping)
			this->StandUp();

		if (choice == SIMON_ATTACK_BY_ROPE)
		{
			// Set up Simon's rope
			rope->SetState(STATE_VISIBLE);
			rope->SetDirection(nx);
		}
	}
}

void CSimon::CalibrateCameraPosition()
{
	float xCam, yCam;
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);

	// get simon's central point
	float xCentral = (l + r) / 2;
	float yCentral = (t + b) / 2;

	float viewportWidth;
	float viewportHeight;
	CGame::GetInstance()->GetViewportSize(viewportWidth, viewportHeight);

	xCam = xCentral - viewportWidth / 2;
	yCam = 0;

	if (xCam < 0)		
		xCam = 0;
	if (xCam + viewportWidth > 4320)	// TO-DO: 1536...
		xCam = 4320 - viewportWidth;

	CGame::GetInstance()->SetCameraPosition(xCam, yCam);
}

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	switch (currentAniID)
	{
	case (int)SimonAniID::CROUCH_RIGHT:
	case (int)SimonAniID::CROUCH_LEFT:
	case (int)SimonAniID::CROUCH_ATTACK_LEFT:
	case (int)SimonAniID::CROUCH_ATTACK_RIGHT:
		if (jumping)
		{
			// While jumping, Simon will have different bounding box
			// Even though the animation is the same
			right = x + SIMON_JUMPING_BBOX_WIDTH;
			bottom = y + SIMON_JUMPING_BBOX_HEIGHT;
		}
		else
		{
			right = x + SIMON_CROUCHING_BBOX_WIDTH;
			bottom = y + SIMON_CROUCHING_BBOX_HEIGHT;
		}
		break;		
	default:
		right = x + SIMON_IDLE_BBOX_WIDTH;
		bottom = y + SIMON_IDLE_BBOX_HEIGHT;
		break;
	}
}


/*
	Tell Simon to do something, if in control
*/
void CSimon::SetAction(Action action)
{
	if (controllable)
	{
		switch (action)
		{
		case Action::WALK_RIGHT:
			this->MoveRight();
			break;
		case Action::WALK_LEFT:
			this->MoveLeft();
			break;
		case Action::CROUCH:
			this->Crouch();
			break;
		case Action::JUMP:
			this->Jump();
			break;
		case Action::ATTACK:
			this->Attack();
			break;
		case Action::USE_ITEM:
			this->UseWeapon();
			break;
		case Action::UPSTAIRS:
			this->Upstairs();
			break;
		case Action::DOWNSTAIRS:
			this->Downstairs();
			break;
		case Action::IDLE:
			this->Idle();
			break;
		default:
			break;
		}
	}
}

void CSimon::SetFreezing(bool freezing)
{
	CGameObject::SetFreezing(freezing);
	rope->SetFreezing(freezing);
}

CSimon * CSimon::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSimon();

	return __instance;
}

