#include <algorithm>

#include "debug.h"
#include "Simon.h"
#include "Game.h"
#include "Animations.h"
#include "BigCandle.h"
#include "ItemRope.h"
#include "Brick.h"

CSimon * CSimon::__instance = NULL;

CSimon::CSimon()
{
	this->attacking = false;
	this->jumping = false;
	this->crouching = false;
	this->attackStartTime = 0;

	// ready to be used
	rope = CRope::GetInstance();
}

void CSimon::SetState(int state)
{
	// Game logic: while attacking, Simon stops moving and reject other action
	// except finishing jumping
	if (attacking)
	{
		if (!jumping)
			vx = 0;
	}

	// Game logic: while jumping, simon can only attack
	else if (jumping)
	{
		if (state == SIMON_STATE_ATTACK && !attacking)
		{
			StartToAttack();

			// re-locate Simon to avoid overlapping the ground
			// because when Simon attacks, it may changes from crouching-height to standing-height
			y += SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
		}
	}

	// Game logic: while crouching, simon can only change direction or attack
	else if (crouching)
	{
		if (state == SIMON_STATE_WALK_RIGHT)
			nx = 1;

		else if (state == SIMON_STATE_WALK_LEFT)
			nx = -1;

		else if (state == SIMON_STATE_ATTACK && !attacking)
			StartToAttack();

		// stop crouching
		else if (state == SIMON_STATE_IDLE)
		{
			crouching = false;
			y += SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
			CGameObject::SetState(SIMON_STATE_IDLE);
		}
	}

	else
	{
		CGameObject::SetState(state);

		switch (state)
		{

		case SIMON_STATE_WALK_RIGHT:
			nx = 1;
			vx = SIMON_WALKING_SPEED;
			break;

		case SIMON_STATE_WALK_LEFT:
			nx = -1;
			vx = -SIMON_WALKING_SPEED;
			break;

		case SIMON_STATE_ATTACK:
			StartToAttack();
			break;

		case SIMON_STATE_JUMP:
			vy = -SIMON_JUMP_SPEED_Y;
			jumping = true;
			break;

		case SIMON_STATE_CROUCH:
			crouching = true;
			vx = 0;
			break;

		case SIMON_STATE_IDLE:
			vx = 0;
			break;
		}
	}
}

void CSimon::Render()
{	
	if (flickering)
	{
		if (argb.blue != 0)
			argb.blue = 0;
		else if (argb.green != 0)
			argb.green = 0;
		else
			argb = ARGB();

		animations->Get(currentAniID)->Render(x, y, argb);
	}
	else
		animations->Get(currentAniID)->Render(x, y);

	//RenderBoundingBox();
}

void CSimon::SetMatchedAnimation(int state)
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

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy
	CMovableObject::Update(dt);

	// Simple fall down
	if (jumping)
	{
		vy += SIMON_JUMP_GRAVITY * dt;
	}		
	else
		vy = WORLD_GRAVITY;

	// Turn off the attacking flag when it'd done
	if (attacking)
		if (GetTickCount() - attackStartTime >= ATTACKING_TIME)
		{
			// To rearrange attacking frames
			this->ResetAnimation(currentAniID);
			attacking = false;
			rope->SetVisible(false);
		}

	// Turn off the flickering flag when it'd done
	if (flickering)
		if (GetTickCount() - flickerStartTime >= FLICKERING_TIME)
			flickering = 0;

	SetMatchedAnimation(state);

	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();

	// Turn off collision when die 
	if (state != SIMON_STATE_DIE)
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

}

void CSimon::ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;

	float min_tx, min_ty, nx, ny;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	// block
	x += min_tx * dx + nx * 0.04f;
	y += min_ty * dy + ny * 0.04f;
	if (ny < 0)
	{
		vy = 0;

		// you are not jumping while your feet on the ground
		if (jumping)
		{
			jumping = false;

			// re-locate Simon to avoid overlapping the ground
			y += SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
		}
	}

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		// Collision logic with BigCandle
		if (dynamic_cast<CBigCandle *>(e->obj))
		{
			CBigCandle * bigCandle = dynamic_cast<CBigCandle *>(e->obj);
			DebugOut(L"\nTouch large candle");

			if (e->nx != 0)
			{
				x -= nx * 0.4f;
			}
			if (e->ny != 0)
			{
				y -= ny * 0.4f;
			}
		}

		else if (dynamic_cast<CItemRope *>(e->obj))
		{
			CItemRope * itemRope = dynamic_cast<CItemRope *>(e->obj);
			DebugOut(L"\nTouch item rope");

			rope->LevelUp();
			itemRope->Destroy();
			this->StartToFlicker();

			/*if (e->nx != 0 || e->ny != 0)
			{
				
			}*/
		}

		// BUG: Simon sometimes ignores the bricks 
		// Block 
		// 
		//else
		//{
		//	if (dynamic_cast<CBrick *>(e->obj))
		//	{
		//		DebugOut(L"\ntouchng brick");
		//	}
		//	if (vx > 0)
		//	{
		//		DebugOut(L"\nvx > 0");
		//	}
		//	x += min_tx * dx + nx * 0.04f;
		//	y += min_ty * dy + ny * 0.04f;
		//	if (nx != 0)
		//		vx = 0;
		//	if (ny != 0)
		//	{
		//		vy = 0;
		//		// you are not jumping while your feet on the ground
		//		if (jumping)
		//			jumping = false;
		//	}
		//}
	}
}

void CSimon::StartToAttack()
{
	if (!attacking)
	{
		attacking = true;
		attackStartTime = GetTickCount();
		
		rope->SetVisible(true);
		rope->SetDirection(nx);
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

CSimon * CSimon::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSimon();

	return __instance;
}

