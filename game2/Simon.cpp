#include <algorithm>

#include "Simon.h"
#include "Game.h"
#include "Animations.h"
#include "BigCandle.h"
#include "ItemRope.h"
#include "Brick.h"
#include "BigHeart.h"
#include "debug.h"

CSimon * CSimon::__instance = NULL;

CSimon::CSimon()
{
	this->attacking = false;
	this->jumping = false;
	this->crouching = false;
	this->attackStartTime = 0;
	this->currentAniID = SIMON_STATE_DIE;

	// ready to be used
	rope = CRope::GetInstance();
}

// TODO: Think twice: 
// Should I use SetAction(Action action) instead of SetState(...) ?
// By that, SetState() will be consistent with all the game objects, 
// only used for set visibile state or invisible state
/*
	Use to tell Simon to do something and process the call
*/
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
		vy += SIMON_JUMP_GRAVITY * dt;
	else
		vy = WORLD_FALL_SPEED;

	// Turn off the attacking flag when it'd done
	// TO-DO: maybe these codes need to be refactoring -> TryEndAttacking(time) & TryEndFlickering(time)
	if (attacking)
		if (GetTickCount() - attackStartTime >= ATTACKING_TIME)
		{
			// To rearrange attacking frames
			this->ResetAnimation(currentAniID);
			attacking = false;
			rope->SetState(STATE_INVISIBLE);
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

	CalibrateCameraPosition();

}

void CSimon::ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;

	float min_tx, min_ty, nx, ny;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	// update x, y to be at the collision position
	x += min_tx * dx;
	y += min_ty * dy;

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		if (dynamic_cast<CBigCandle *>(e->obj))
		{
			CBigCandle * bigCandle = dynamic_cast<CBigCandle *>(e->obj);
			DebugOut(L"\n[INFO] Touch large candle");
		}

		else if (dynamic_cast<CBigHeart *>(e->obj))
		{
			CGameObject * obj = dynamic_cast<CBigHeart *>(e->obj);
			DebugOut(L"\n[INFO] Touch item heart rope");

			obj->SetState(STATE_INVISIBLE);
		}

		else if (dynamic_cast<CItemRope *>(e->obj))
		{
			CGameObject * obj = dynamic_cast<CItemRope *>(e->obj);
			DebugOut(L"\n[INFO] Touch item rope");

			this->rope->LevelUp();
			this->StartToFlicker();

			obj->SetState(STATE_INVISIBLE);
		}

		// block with ground objects
		else
		{
			x += nx * 0.4f;
			y += ny * 0.4f;

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
		}
	}
}

void CSimon::StartToAttack()
{
	if (!attacking)
	{
		attacking = true;
		attackStartTime = GetTickCount();
		
		rope->SetState(STATE_VISIBLE);
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

CSimon * CSimon::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSimon();

	return __instance;
}

