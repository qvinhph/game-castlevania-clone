#include "Fish.h"
#include "InvisibleWall.h"
#include "Simon.h"


void CFish::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + FISH_BBOX_WIDTH;
	bottom = top + FISH_BBOX_HEIGHT;	
}

void CFish::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);

	// First appearance 
	if (firstAppear == true)
		this->Jump();
	

	// Fall down
	vy += (jumping) ?
		FISH_JUMP_GRAVITY * dt :
		GAME_GRAVITY * dt;


	// Attack when on time
	if (attack_pause == TIMER_IDLE)
	{
		this->Attack();
		attack_pause = GetTickCount();
	}

	// Not on time to attack -> Keep walking
	else
	{
		this->ProceedAttacking();
		this->ProceedPausingAttacking();
		this->Move();
	}


	// Choose matched animation
	PickAnimation();
	

	// Collisions
	vector<LPCOLLISIONEVENT> coEvents;
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
		ProceedCollisions(coEvents);
	   
	// When go out of viewport
	if (this->IsInViewport() == false)
		SetState(STATE_INVISIBLE);
}

void CFish::ProceedCollisions(vector<LPCOLLISIONEVENT>& coEvents)
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

		if (dynamic_cast<CInvisibleWall *>(e->obj))
		{
			if (e->nx != 0)
			{
				x += nx * DEFLECTION_AVOID_OVERLAPPING;
			}

			if (e->ny < 0)
			{
				// Remove jumping flag
				jumping = false;

				// Prevent overlapping next time
				y += ny * DEFLECTION_AVOID_OVERLAPPING;
				vy = 0;
			}
		}
		else
		{
			// Ignore other objects by completing the rest of dx / dy
			if (e->nx != 0)	x += (1 - min_tx) * dx;
			if (e->ny < 0)	y += (1 - min_ty) * dy;
		}
	}

}

void CFish::Jump()
{
	vy = -FISH_JUMP_SPEED;
	firstAppear = false;
	jumping = true;
}

void CFish::Attack()
{
	if (attack_start == TIMER_IDLE && jumping == false)
	{
		vx = 0;
		attack_start = GetTickCount();

		// Call the fire ball
		CWeapons::GetInstance()->AddToStock(Weapon::FIREBALL);
		CWeapons::GetInstance()->UseWeapon(fireball, this);
	}
}

void CFish::Move()
{
	if (!jumping &&
		attack_start == TIMER_IDLE)
	{
		this->vx = (nx > 0) ?
			FISH_MOVE_SPEED :
			-FISH_MOVE_SPEED;
	}
}

void CFish::ProceedAttacking()
{
	if (attack_start != TIMER_IDLE &&
		attack_start != TIMER_ETERNAL)
	{
		if (GetTickCount() - attack_start > FISH_ATTACK_TIME)
		{
			attack_start = TIMER_IDLE;
			attack_pause = GetTickCount();

			// Change direction to Simon after doing the attack
			float xSimon, ySimon;
			CSimon::GetInstance()->GetPosition(xSimon, ySimon);
			this->ReDirect(xSimon);
		}
	}
}

void CFish::ProceedPausingAttacking()
{
	if (attack_pause != TIMER_IDLE &&
		attack_pause != TIMER_ETERNAL)
	{
		if (GetTickCount() - attack_pause > FISH_PAUSE_ATTACK_TIME)
		{
			attack_pause = TIMER_IDLE;
		}
	}
}

void CFish::PickAnimation()
{
	if (jumping)
	{
		currentAniID = (nx > 0) ?
			(int)FishAniID::IDLE_RIGHT :
			(int)FishAniID::IDLE_LEFT;
	}
	else if (attack_start != TIMER_IDLE)
	{
		currentAniID = (nx > 0) ?
			(int)FishAniID::ATTACK_RIGHT :
			(int)FishAniID::ATTACK_LEFT;
	}
	else if (vx != 0)
	{
		currentAniID = (vx > 0) ?
			(int)FishAniID::MOVE_RIGHT : 
			(int)FishAniID::MOVE_LEFT;
	}
}

CFish::CFish()
{
	point = FISH_POINT_AWARD;
	health = FISH_DEFAULT_HEALTH;
	damage = FISH_DAMAGE;
}
