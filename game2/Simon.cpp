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

#include "debug.h"

CSimon * CSimon::__instance = NULL;

CSimon::CSimon()
{
	this->attacking = false;
	this->jumping = false;
	this->crouching = false;
	this->secondWeapon = Weapon::NONE;

	// ready to be used
	rope = CRope::GetInstance();
	weapons = CWeapons::GetInstance();
}

void CSimon::Render()
{	
	if (flickering)
	{
		// When flickering, animation frame will be render with
		// these 3 color

		if (this->argb.blue != 0)
			this->argb.blue = 0;

		else if (argb.green != 0)
			this->argb.green = 0;

		else
			this->argb = ARGB();
	}
	
	CGameObject::Render();
}

void CSimon::SetMatchedAnimation()
{
	if (attacking)
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

	// these two action use the same animation
	else if (crouching || jumping)
	{
		currentAniID = (nx > 0) ?
			(int)SimonAniID::CROUCH_RIGHT :
			(int)SimonAniID::CROUCH_LEFT;
	}

	else if (vx > 0)
	{
		nx = 1;
		currentAniID = (int)SimonAniID::WALK_RIGHT;
	}

	else if (vx < 0)
	{
		nx = -1;
		currentAniID = (int)SimonAniID::WALK_LEFT;
	}

	else if (vx == 0)
	{
		currentAniID = (nx > 0) ?
			(int)SimonAniID::IDLE_RIGHT :
			(int)SimonAniID::IDLE_LEFT;
	}
}

void CSimon::StartToAttack(Weapon secondWeapon)
{
	if (!attacking)
	{
		attacking = true;
		attackStartTime = GetTickCount();

		if (secondWeapon == Weapon::NONE)
		{
			rope->SetState(STATE_VISIBLE);
			rope->SetDirection(nx);
		}
	}
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy
	CMovableObject::Update(dt);

	// Simple fall down
	if (jumping)
		vy += SIMON_JUMP_GRAVITY * dt;
	else
		vy += GAME_GRAVITY * dt;

	// Turn off the attacking flag when it'd done
	// TO-DO: maybe these codes need to be refactoring -> TryEndAttacking(time) & TryEndFlickering(time)
	if (attacking)
		if (GetTickCount() - attackStartTime >= ATTACKING_TIME)
		{
			// If used rope to attack
			if (rope->GetState() == STATE_VISIBLE)
				rope->SetState(STATE_INVISIBLE);
			else 
				weapons->UseWeapon(secondWeapon, this);


			// To rearrange attacking frames
			this->ResetAnimation(currentAniID);
			attacking = false;			
		}

	// Turn off the flickering flag when it'd done
	if (flickering)
		if (GetTickCount() - flickerStartTime >= FLICKERING_TIME)
		{
			flickering = 0;
			argb = ARGB();
		}

	SetMatchedAnimation();

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

		// Can remove this
		if (dynamic_cast<CBigCandle *>(e->obj) ||
			dynamic_cast<CFlame *>(e->obj) ||
			dynamic_cast<CDagger *>(e->obj))
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
			this->StartToFlicker();
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

		// block with ground objects
		else if (dynamic_cast<CInvisibleWall *>(e->obj))
		{
			// Prevent overlapping next frame
			x += nx * COLLISION_FORCE;		
			y += ny * COLLISION_FORCE;

			if (ny < 0)
			{
				vy = 0;

				// Simon is not jumping while his feet on the ground
				if (jumping)
				{
					jumping = false;

					// re-locate Simon to avoid overlapping the ground
					y += SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
				}
			}
		}
	}
}

void CSimon::StartToFlicker()
{
	if (!flickering)
	{
		flickering = true;
		flickerStartTime = GetTickCount();
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
		right = x + SIMON_CROUCHING_BBOX_WIDTH;
		bottom = y + SIMON_CROUCHING_BBOX_HEIGHT;
		break;
	default:
		right = x + SIMON_IDLE_BBOX_WIDTH;
		bottom = y + SIMON_IDLE_BBOX_HEIGHT;
		break;
	}
}

void CSimon::SetAction(Action action)
{
	// Simon behavior: while attacking, Simon stops moving and reject other action
	// except finishing jumping
	if (attacking)
	{
		if (!jumping)
			vx = 0;
	}

	// Simon behavior: while jumping, simon can only attack
	// TO-DO: Need more thought on this (jumping)
	else if (jumping)
	{
		if (!attacking)
		{
			if (action == Action::ATTACK)
			{
				StartToAttack();

				// Divide by 2 to keep the attack while jumping action smoothly 
				// and also to make this similar to the origin game
				y += (SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT) / 2;
			}
			else if (action == Action::SECOND_ATTACK)
			{
				StartToAttack(secondWeapon);
				y += (SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT) / 2;
			}
		}
		
	}

	// Simon behavior: while crouching, simon can only change direction or attack
	else if (crouching)
	{
		if (action == Action::WALK_RIGHT)
			nx = 1;

		else if (action == Action::WALK_LEFT)
			nx = -1;

		else if (action == Action::ATTACK && !attacking)
			StartToAttack();

		// stop crouching
		else if (action == Action::IDLE)
		{
			crouching = false;
			y += SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
		}
	}

	else
	{
		switch (action)
		{
		case Action::WALK_RIGHT:
			nx = 1;
			vx = SIMON_WALKING_SPEED;
			break;

		case Action::WALK_LEFT:
			nx = -1;
			vx = -SIMON_WALKING_SPEED;
			break;

		case Action::SECOND_ATTACK:
			StartToAttack(secondWeapon);
			break;

		case Action::ATTACK:
			StartToAttack();
			break;

		case Action::JUMP:
			vy = -SIMON_JUMP_SPEED_Y;
			jumping = true;
			break;

		case Action::CROUCH:
			crouching = true;
			vx = 0;
			break;

		case Action::IDLE:
			vx = 0;
			break;

		default:
			DebugOut(L"[ERROR] Simon cannot perform this action (Action enum: %d)", (int)action);
			return;
		}

		CMovableObject::SetAction(action);
	}
}

CSimon * CSimon::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSimon();

	return __instance;
}

