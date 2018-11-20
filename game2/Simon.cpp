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

#include "debug.h"
#include <iostream>

CSimon * CSimon::__instance = NULL;

CSimon::CSimon()
{
	this->controllable = true;
	this->jumping = false;
	this->crouching = false;
	this->secondWeapon = Weapon::NONE;

	auto_crouch_start = 0;
	attack_start = 0;
	flicker_start = 0;

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

void CSimon::SetMatchedAnimation()
{
	if (attack_start != 0)
	{
		if (crouching)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::CROUCH_ATTACK_RIGHT :
				(int)SimonAniID::CROUCH_ATTACK_LEFT;
		else
			currentAniID = (nx > 0) ?
				(int)SimonAniID::ATTACK_RIGHT :
				(int)SimonAniID::ATTACK_LEFT;
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
	CMovableObject::Update(dt);
	

	// While jumping and falling
	if (jumping)
	{
		if (vy > SIMON_MAX_SPEED_BY_JUMP_GRAVITY)
		{
			this->StandUp();
			controllable = false;
		}
		else
			vy += SIMON_JUMP_GRAVITY * dt;
	}
	else
	{
		// If not jumping:
		// vy will always be "!= 0" when Simon is falling
		// Otherwise, vy's Simon is zero and not zero one by one
		// Because of collision with the ground
		if (vy != 0)
			controllable = false;

		vy += SIMON_FALL_GRAVITY * dt;

		//if (controllable == false) DebugOut(L"\n...");
		//if (controllable == true) DebugOut(L"\n000");
	}

	if (vy > SIMON_MAX_SPEED_Y)
		vy = SIMON_MAX_SPEED_Y;

	// Auto crouch
	if (auto_crouch_start > 0)
		if (GetTickCount() - auto_crouch_start >= AUTO_CROUCH_TIME)
		{
			this->StandUp();

			// Stop counting
			auto_crouch_start = 0;
		}

	// Attacking
	if (attack_start > 0)
		if (GetTickCount() - attack_start > ATTACK_TIME)
		{
			// If attacking by rope
			if (this->rope->GetState() == STATE_VISIBLE)
				this->rope->SetState(STATE_INVISIBLE);
			else
				weapons->UseWeapon(secondWeapon, this);

			this->ResetAnimation(currentAniID);
			attack_start = 0;
		}

	// Being Untouchable
	if (untouchable_start != -1 &&
		untouchable_start != 0)
		if (GetTickCount() - untouchable_start > UNTOUCHABLE_TIME)
		{
			// Stop flickering effect
			this->argb = ARGB();
			flicker_start = 0;

			untouchable_start = 0;
		}

	// Flickering
	if (flicker_start > 0)
		if (GetTickCount() - flicker_start >= FLICKERING_TIME)
		{
			this->argb = ARGB();
			flicker_start = 0;
		}

	SetMatchedAnimation();

	// Collisions
	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();

	// Turn off collision when die 
	if (this->action != Action::DIE)
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

void CSimon::ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;
	float min_tx, min_ty, nx, ny;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	// update x, y to make object be right at the collision position
	// for ignoring collision with some kind of objects
	x += min_tx * dx;
	y += min_ty * dy;

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		if (dynamic_cast<CBigCandle *>(e->obj) ||
			dynamic_cast<CFlame *>(e->obj) ||
			dynamic_cast<CDagger *>(e->obj) ||
			dynamic_cast<CCandle *>(e->obj))
			DebugOut(L"\n[INFO] Touch something but do nothing !!");

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

		else if (dynamic_cast<CZombie *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Zombie");
			if (this->OnGetDamaged(e))
				return;
		}

		// block with ground objects
		else if (dynamic_cast<CInvisibleWall *>(e->obj))
		{
			// Prevent overlapping next frame
			if (nx != 0)
				x += nx * DEFLECTION_AVOID_OVERLAPPING;					

			if (ny < 0)
			{

				if (jumping)	
					this->StandUp();		
				else			
					// Regain the control after falling
					controllable = true;	

				// When falls from too high place
				if (vy == SIMON_MAX_SPEED_Y || untouchable_start == -1)
				{
					AutoCrouch();

					if (untouchable_start == -1)
						this->BeUntouchable();
				}

				y += ny * DEFLECTION_AVOID_OVERLAPPING;
				vy = 0;				
			}
		}
	}
}

void CSimon::Flicker()
{
	if (flicker_start == 0)
		flicker_start = GetTickCount();
}

/*
	The function helps Simon stand up without overlapping 
	objects by re-locating Simon position
*/
void CSimon::StandUp()
{
	if (crouching)
	{
		crouching = false;
		y += SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
	}
	else if (jumping)
	{
		if (attack_start == 0)
			jumping = false;

		y += SIMON_JUMPING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
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
	Use this to start an untouchable state with soon ending.
*/
void CSimon::BeUntouchable()
{
	if (untouchable_start == 0 ||
		untouchable_start == -1)
	{
		untouchable_start = GetTickCount();
		this->Flicker();
	}
}

/*
	Tell Simon to be get damaged.
	Return 1 if he obeys.
	Otherwise, return 0.
*/
int CSimon::OnGetDamaged(LPCOLLISIONEVENT e)
{
	if (untouchable_start != 0)
		return 0;

	this->nx = (e->nx != 0) ?
		-(e->nx) :
		-(e->obj->GetDirection());

	vx = vy = dx = dy = 0;

	this->vx = (-this->nx) * SIMON_DAMAGED_DEFLECT_X;
	this->vy = SIMON_DAMAGED_DEFLECT_Y;
	jumping = true;
	controllable = false;

	// Simon is being untouchable but not going to stop being untouchable
	untouchable_start = -1;

	return 1;
}

void CSimon::MoveRight()
{
	if (attack_start == 0 &&	// While attacking, Simon cannot move
		!jumping)				// While jumping, Simon cannot move
	{
		this->nx = 1;
		if (!crouching)
			this->vx = SIMON_WALKING_SPEED * this->nx;
	}
}

void CSimon::MoveLeft()
{
	if (attack_start == 0 &&	// While attacking, Simon cannot move
		!jumping)				// While jumping, Simon cannot move
	{
		this->nx = -1;
		if (!crouching)
			this->vx = SIMON_WALKING_SPEED * this->nx;
	}
}

void CSimon::Crouch()
{
	if (!crouching && 
		attack_start == 0)
	{
		this->crouching = true;
		this->vx = 0;

		// Re-locate Simon
		this->y += SIMON_IDLE_BBOX_HEIGHT - SIMON_CROUCHING_BBOX_HEIGHT;
	}	
}

void CSimon::Idle()
{
	if (!jumping)
	{
		this->vx = 0;

		if (crouching && auto_crouch_start == 0)
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
	

void CSimon::Jump()
{
	if (!jumping)
	{
		this->jumping = true;
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
		else 
			vx = 0;

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
	CGame::GetInstance()->GetViewportSize(viewportHeight, viewportWidth);
	xCam = xCentral - viewportWidth / 2;

	// TODO: should this need a more generic code ?
	yCam = 0;//yCentral - viewportHeight / 2;

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

