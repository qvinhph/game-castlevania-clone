#include <algorithm>

#include "debug.h"
#include "Simon.h"
#include "Game.h"
#include "Animations.h"

CSimon * CSimon::__instance = NULL;

CSimon::CSimon()
{
	this->attacking = false;
	this->jumping = false;
	this->crouching = false;
	this->attacking_start_time = 0;

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
	if (attacking)
	{
		if (crouching)
			currentAniID = (nx > 0) ? SimonAniID::CROUCH_ATTACK_RIGHT : SimonAniID::CROUCH_ATTACK_LEFT;
		else
			currentAniID = (nx > 0) ? SimonAniID::ATTACK_RIGHT : SimonAniID::ATTACK_LEFT;
	}

	// these two action use the same animation
	else if (crouching || jumping)
	{
		currentAniID = (nx > 0) ? SimonAniID::CROUCH_RIGHT : SimonAniID::CROUCH_LEFT;

		/*if (jumping && vy >= 0)
		{
			ani = (nx > 0) ? SimonAniID::IDLE_RIGHT : SimonAniID::IDLE_LEFT;
		}*/
	}

	else if (vx > 0)
	{
		nx = 1;
		currentAniID = SimonAniID::WALK_RIGHT;
	}

	else if (vx < 0)
	{
		nx = -1;
		currentAniID = SimonAniID::WALK_LEFT;
	}

	else if (vx == 0)
	{
		currentAniID = (nx > 0) ? SimonAniID::IDLE_RIGHT : SimonAniID::IDLE_LEFT;
	}

	
	//simonAnimations[ani]->Render(x, y);

	RenderAnimation(currentAniID);
	//RenderBoundingBox();
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy
	CMovableObject::Update(dt);

	// Simple fall down
	if (jumping)
		vy += SIMON_GRAVITY * dt;
	else
		vy = 1.0f;	// similar to the game logic


	// Turn off the attacking flag when it'd done
	if (attacking)
		if (GetTickCount() - attacking_start_time >= ATTACKING_TIME)
		{
			attacking = false;
			rope->SetVisible(false);

			// To make sure that every frame is in their order again
			this->ResetAnimation(currentAniID);
		}


	//
	// Collision
	//
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

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
	{
		float min_tx, min_ty, nx, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// Handling the collision
		// Block ( almost with the ground )
		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;

		if (nx != 0) vx = 0;
		if (ny != 0)
		{
			vy = 0;
			jumping = false; // while your feet touch the ground, you are not jumping
		}

		// Collision logic with Ghosts
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSimon::StartToAttack()
{
	if (!attacking)
	{
		attacking = true;
		attacking_start_time = GetTickCount();
		
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

