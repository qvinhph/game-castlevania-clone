#include <algorithm>

#include "debug.h"
#include "Simon.h"
#include "Game.h"
#include "Animations.h"
#include "BigCandle.h"

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
		return;
	}

	// Game logic: while jumping, simon can only attack
	if (jumping)
	{
		if (state == SIMON_STATE_ATTACK && !attacking)
			StartToAttack();
		
		return;
	}

	// Game logic: while crouching, simon can only change direction or attack
	if (crouching)
	{
		if (state == SIMON_STATE_WALK_RIGHT)
			nx = 1;

		else if (state == SIMON_STATE_WALK_LEFT)
			nx = -1;

		else if (state == SIMON_STATE_ATTACK && !attacking)
			StartToAttack();

		else if (state == SIMON_STATE_IDLE)
		{
			crouching = false;
			y += SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
		}

		return;
	}

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
		if (crouching)
		{
			crouching = false;
			y += SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
		}
		break;
	}
}

void CSimon::Render()
{	
	RenderAnimation(currentAniID);
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

	SetMatchedAnimation(state);

	// Simple fall down
	if (jumping)
		vy += SIMON_GRAVITY * dt;
	else
		vy = 1.0f;	// similar to the game logic
			

	// Turn off the attacking flag when it'd done
	if (attacking)
		if (GetTickCount() - attackStartTime >= ATTACKING_TIME)
		{
			attacking = false;
			rope->SetVisible(false);

			// To make sure that every frame is in their order again
			this->ResetAnimation(currentAniID);
		}

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
		ProcessCollision(coEvents);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CSimon::ProcessCollision(vector<LPCOLLISIONEVENT> &coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;

	float min_tx, min_ty, nx, ny;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		// Collision logic with BigCandle
		if (dynamic_cast<CBigCandle *>(e->obj))
		{
			CBigCandle * bigCandle = dynamic_cast<CBigCandle *>(e->obj);
			DebugOut(L"Touching candle \n");

			if (e->nx != 0)
			{
				x += min_tx * dx;
			}
			if (e->ny != 0)
			{
				y += min_ty * dy;
			}
		}

		// Block 
		else
		{
			x += min_tx * dx + nx * 0.04f;
			y += min_ty * dy + ny * 0.04f;

			if (nx != 0)
				vx = 0;

			if (ny != 0)
			{
				vy = 0;

				// while your feet on the ground, you are not jumping
				jumping = false;
			}
		}
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

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	if (crouching)
	{
		right = x + SIMON_CROUCHING_BBOX_WIDTH;
		bottom = y + SIMON_CROUCHING_BBOX_HEIGHT;
	}
	else
	{
		right = x + SIMON_IDLE_BBOX_WIDTH;
		bottom = y + SIMON_IDLE_BBOX_HEIGHT;
	}
}

CSimon * CSimon::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSimon();

	return __instance;
}

