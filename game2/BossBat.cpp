#include "BossBat.h"
#include "Simon.h"
#include "Board.h"
#include "Camera.h"

void CBossBat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + BOSSBAT_BBOX_WIDTH;
	bottom = y + BOSSBAT_BBOX_HEIGHT;
}

void CBossBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);

	// Pick matched animation
	currentAniID = (sleeping) ?
		(int)BossBatAniID::SLEEP :
		(int)BossBatAniID::FLY;

	
	// Update the current boss's central position
	CalculateCentralPosition();


	// Find player
	FindTarget();


	// Try to wake up
	if (sleeping)
	{
		if (CanWakeUp())
		{
			sleeping = false;
			rest_start = GetTickCount();
			//CalculateRestPlace();
		}
		else
			return;
	}
	   
	// ORDER OF BAT'S ACTOIN IF NOT SLEEP: 
	// REST -> ATTACK -> GO_TO_REST -> REST -> (again...)
	// Resting
	if (rest_start != TIMER_IDLE)
	{
		if (GetTickCount() - rest_start > BOSSBAT_REST_TIME)
			rest_start = TIMER_IDLE;
	}

	// Not resting
	else
	{
		// 
		if (goingToRest)
			GoToRest();

		else
		{
			if (attack_start == TIMER_IDLE)
			{
				StartAttack();
			}
			else
			{
				ProceedAttacking();
			}
		}
	}		

	// Keep the BossBat being inside the viewport
	float left, top, right, bottom;
	CCamera::GetInstance()->GetBoundingBox(left, top, right, bottom);
	if (x < left && x + BOSSBAT_BBOX_WIDTH > right)
		vx = -vx;
	if (y < top && y + BOSSBAT_BBOX_HEIGHT > bottom)
		vx = -vy;

	// Update positon 
	x += dx;
	y += dy;
}

bool CBossBat::IsInViewport()
{
	// The viewport bounding box
	float vpLeft, vpTop, vpRight, vpBottom;
	cameraInstance->GetBoundingBox(vpLeft, vpTop, vpRight, vpBottom);

	// The object bounding box
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);

	if (vpLeft > right || vpTop > bottom
		|| vpRight < left || vpBottom < top)
		return false;

	return true;
}

void CBossBat::BeHit(int damage)
{
	if (untouchable_start == TIMER_IDLE)
		CBoard::GetInstance()->AddEnemyLife(-damage);

	CActiveObject::BeHit(damage);
}

/*
	Try to turn off the timer attack if it ran out
*/
void CBossBat::ProceedAttacking()
{
	// If run out of an attack time
	if (GetTickCount() - attack_start > BOSSBAT_ATTACK_TIME)
	{
		// Stop timer
		attack_start = TIMER_IDLE;

		// Stop movement
		vx = vy = 0;

		// Going to rest ( will move with lower speed )
		goingToRest = true;
		CalculateRestPlace();
	}

	// Keep decreasing the Boss's vy to create flying -up-movement
	else 
		vy += BOSSBAT_FLY_UP_SPEED_VY;
}

void CBossBat::StartAttack()
{
	if (attack_start == TIMER_IDLE)
	{
		// Vector speed to run into Simon for an attack
		vx = (target_x - central_x) / (BOSSBAT_ATTACK_TIME / 2);
		vy = (target_y - central_y) / (BOSSBAT_ATTACK_TIME / 2);

		// To keep the bat move not too fast
		vx = (vx > 0) ?
			(vx > BOSSBAT_MAX_SPEED_VX) ? BOSSBAT_MAX_SPEED_VX : vx :
			(vx < -BOSSBAT_MAX_SPEED_VX) ? -BOSSBAT_MAX_SPEED_VX : vx;

		vy = (vy > 0) ?
			(vy > BOSSBAT_MAX_SPEED_VY) ? BOSSBAT_MAX_SPEED_VY : vy :
			(vy < -BOSSBAT_MAX_SPEED_VY) ? -BOSSBAT_MAX_SPEED_VY : vy;

		// Start timer for attacking action
		attack_start = GetTickCount();
	}
}

void CBossBat::GoToRest()
{
	if (ReachedRestPlace())
	{
		goingToRest = false;
		Rest();
	}
	else
	{
		// Going to rest with those speed (vx, vy)
		vx = (central_x < rest_x) ?
			BOSSBAT_GO_TO_REST_SPEED_VX :
			-BOSSBAT_GO_TO_REST_SPEED_VX;

		vy = (central_y < rest_y) ?
			BOSSBAT_GO_TO_REST_SPEED_VY :
			-BOSSBAT_GO_TO_REST_SPEED_VY;
	}
		
}

void CBossBat::Rest()
{
	if (rest_start == TIMER_IDLE)
	{
		rest_start = GetTickCount();
		vx = vy = 0;
	}
}

CBossBat::CBossBat()
{
	health = BOSSBAT_DEFAULT_HEALTH;
	damage = BOSSBAT_DAMAGE;
	point = BOSSBAT_POINT_AWARD;
}

/*
	Calculate the central position of the boss bat.
*/
void CBossBat::CalculateCentralPosition()
{
	central_x = x + BOSSBAT_BBOX_WIDTH / 2;
	central_y = y + BOSSBAT_BBOX_HEIGHT / 2;
}

void CBossBat::CalculateRestPlace()
{
	rest_x = (central_x + target_x) / 2;
	rest_y = (central_y + target_y) / 2;


	// Make sure the BossBat have a good distance from Simon ( for good player's experience)
	if (abs(rest_x - target_x) < BOSSBAT_MINIMUM_DISTANCE_WITH_SIMON)
	{
		if (rest_x < target_x)
			rest_x = target_x - BOSSBAT_MINIMUM_DISTANCE_WITH_SIMON;
		else
		{
			rest_x = target_x + BOSSBAT_MINIMUM_DISTANCE_WITH_SIMON;
		}		
	}

	if (abs(rest_y - target_y) < BOSSBAT_MINIMUM_DISTANCE_WITH_SIMON)
	{
		rest_y = target_y - BOSSBAT_MINIMUM_DISTANCE_WITH_SIMON;
	}

	// Keep the BossBat being inside viewport
	float left, top, right, bottom;
	CCamera::GetInstance()->GetBoundingBox(left, top, right, bottom);
	if (rest_x > right)		rest_x = target_x - BOSSBAT_MINIMUM_DISTANCE_WITH_SIMON;
	if (rest_x < left)		rest_x = target_x + BOSSBAT_MINIMUM_DISTANCE_WITH_SIMON;

	if (rest_y < top && rest_y > bottom)		
		rest_y = target_y - BOSSBAT_MINIMUM_DISTANCE_WITH_SIMON;
}


/*
	Calculate the central position of the target ( Simon )
*/
void CBossBat::FindTarget()
{
	float left, top, right, bottom;
	CSimon::GetInstance()->GetBoundingBox(left, top, right, bottom);

	target_x = (left + right) / 2;
	target_y = (top + bottom) / 2;
}

/*
	Check if start to fight
*/
bool CBossBat::CanWakeUp()
{
	if (target_x > central_x)
		return true;

	return false;
}

bool CBossBat::ReachedRestPlace()
{
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);

	if (left < rest_x && right > rest_x &&
		top < rest_y && bottom > rest_y)
		return true;

	return false;
}
