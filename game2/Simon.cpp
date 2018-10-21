#include <algorithm>

#include "debug.h"
#include "Simon.h"
#include "Game.h"


CSimon::CSimon() : CGameObject()
{
	this->attacking = 0;
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		nx = 1;
		vx = SIMON_WALKING_SPEED;
		break;

	case SIMON_STATE_WALKING_LEFT:
		nx = -1;
		vx = -SIMON_WALKING_SPEED;
		break;

	case SIMON_STATE_IDLE:
		vx = 0;
		break;

	case SIMON_STATE_ATTACKING:
		StartToAttack();
		break;

	case SIMON_STATE_JUMPING:
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	}
}

void CSimon::StartToAttack()
{
	if (attacking == 0)
	{
		attacking = 1;
		attacking_start_time = GetTickCount();
		flag = false;
	}
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY;
	if (y >= 300)
	{
		vy = 0; y = 300.0f;
	}

	//
	// Collision
	//


	// Normally update the position of simon
	x += dx;
	y += dy;


	// Turn off the attacking flag when it'd done
	if (GetTickCount() - attacking_start_time > ATTACKING_TIME)
		attacking = 0;
}

void CSimon::Render()
{
	int ani;


	if (attacking)
	{
		ani = (nx > 0) ? SimonAniID::ATTACK_RIGHT : SimonAniID::ATTACK_LEFT;
	}

	else if (vx > 0)
	{
		nx = 1;
		ani = SimonAniID::WALK_RIGHT;
	}

	else if (vx < 0)
	{
		nx = -1;
		ani = SimonAniID::WALK_LEFT;
	}

	else if (vx == 0)
	{
		ani = (nx > 0) ? SimonAniID::IDLE_RIGHT : SimonAniID::IDLE_LEFT;
	}

	DebugOut(L"\nAni: %d", ani);
	animations[ani]->Render(x, y);
}

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	if (state == SIMON_STATE_IDLE)
	{
		right = x + SIMON_IDLE_BBOX_WIDTH;
		bottom = y + SIMON_IDLE_BBOX_HEIGHT;
	}
	else
	{
		right = x + SIMON_CROUCHING_BBOX_WIDTH;
		bottom = y + SIMON_CROUCHING_BBOX_HEIGHT;
	}
}
