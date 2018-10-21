#include <algorithm>

#include "debug.h"
#include "Simon.h"
#include "Game.h"
#include "Animations.h"

unordered_map<int, LPANIMATION> CSimon::simonAnimations;

CSimon::CSimon() : CMovableObject()
{
	this->attacking = 0;
}

void CSimon::SetState(int state)
{
	CMovableObject::SetState(state);

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

	case SIMON_STATE_IDLE:
		vx = 0;
		break;

	case SIMON_STATE_ATTACK:
		StartToAttack();
		break;

	case SIMON_STATE_JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	}
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy
	CMovableObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	//
	// Collision
	//
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// Turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// Turn off the attacking flag when it'd done
	if (GetTickCount() - attacking_start_time > ATTACKING_TIME)
		attacking = 0;

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
		// Block
		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Ghosts
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
	simonAnimations[ani]->Render(x, y);
}

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + SIMON_IDLE_BBOX_WIDTH;
	bottom = y + SIMON_IDLE_BBOX_HEIGHT;
	/*if (state)
	{
		right = x + SIMON_IDLE_BBOX_WIDTH;
		bottom = y + SIMON_IDLE_BBOX_HEIGHT;
	}*/
	/*else
	{
		right = x + SIMON_CROUCHING_BBOX_WIDTH;
		bottom = y + SIMON_CROUCHING_BBOX_HEIGHT;
	}*/
}

void CSimon::AddAnimation(int aniID)
{
	simonAnimations[aniID] = CAnimations::GetInstance()->Get(aniID);
}

void CSimon::StartToAttack()
{
	if (attacking == 0)
	{
		attacking = 1;
		attacking_start_time = GetTickCount();
	}
}

