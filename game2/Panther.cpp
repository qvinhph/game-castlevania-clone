#include "Panther.h"
#include "Simon.h"
#include "InvisibleWall.h"

void CPanther::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	if (currentAniID == (int)PantherAniID::IDLE_LEFT ||
		currentAniID == (int)PantherAniID::IDLE_RIGHT)
	{
		right = left + PANTHER_IDLE_BBOX_WIDTH;
		bottom = top + PANTHER_IDLE_BBOX_HEIGHT;
	}

	//else if (currentAniID == (int)PantherAniID::RUN_LEFT ||
	//		currentAniID == (int)PantherAniID::RUN_RIGHT)
	else
	{
		right = left + PANTHER_RUN_BBOX_WIDTH;
		bottom = top + PANTHER_RUN_BBOX_HEIGHT;
	}
	
}

void CPanther::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);


	// Simple fall down
	vy += PANTHER_FALL_GRAVITY;
	if (CheckIfNeedJump() && vx != 0)
	{
		Jump();
		groundStandingOn = NULL;
	}


	// Looking for Simon while idle
	if (FindSimon(PANTHER_FIND_RADIUS) && vx == 0)
	{
		ChangeDirectonToSimon();

		vx = (nx > 0) ?
			PANTHER_RUN_SPEED :
			-PANTHER_RUN_SPEED;
	}		
		

	PickAnimation();


	// Collisions
	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else ProceedCollisions(coEvents);


	for (UINT i = 0; i < coEvents.size(); ++i) delete coEvents[i];
}

bool CPanther::FindSimon(float radius)
{
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	if (abs(x - xS) < radius)
		return true;

	if (nx == 1)
		if (abs(x - xS) - PANTHER_IDLE_BBOX_WIDTH < radius)
			return true;

	return false;
}

bool CPanther::CheckIfNeedJump()
{
	if (groundStandingOn != NULL)
	{
		float l, t, r, b;
		groundStandingOn->GetBoundingBox(l, t, r, b);

		float left, top, right, bottom;
		this->GetBoundingBox(left, top, right, bottom);

		if (left < l || right > r)
			return true;
	}

	return false;
}

void CPanther::Jump()
{
	if (!jumping)
	{
		vy = -PANTHER_JUMP_SPEED;
		jumping = true;

		// Re-calculate the dy for checking collision
		dy = this->dt * vy;
	}
}

void CPanther::ChangeDirectonToSimon()
{
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	ReDirect(xS);
}

void CPanther::PickAnimation()
{
	if (jumping)
	{
		currentAniID = (nx > 0) ?
			(int)PantherAniID::JUMP_RIGHT :
			(int)PantherAniID::JUMP_LEFT;
	}
	else if (vx > 0)
	{
		currentAniID = (int)PantherAniID::RUN_RIGHT;
	}
	else if (vx < 0)
	{
		currentAniID = (int)PantherAniID::RUN_LEFT;
	}
	else if (vx == 0)
	{
		currentAniID = (nx > 0) ?
			(int)PantherAniID::IDLE_RIGHT :
			(int)PantherAniID::IDLE_LEFT;
	}
		
}

void CPanther::ProceedCollisions(vector<LPCOLLISIONEVENT>& coEvents)
{
	float min_tx, min_ty, nx, ny;
	vector<LPCOLLISIONEVENT> coEventsResult;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	// Set the object's position right to the point occured collision
	x += min_tx * dx;
	y += min_ty * dy;

	for (UINT i = 0; i < coEventsResult.size(); ++i)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		if (dynamic_cast<CInvisibleWall *>(e->obj))
		{
			if (e->nx != 0)
			{
				x += DEFLECTION_AVOID_OVERLAPPING * nx;
				vx = -vx;
			}

			if (e->ny < 0)
			{
				if (vx != 0 && jumping)
				{
					ChangeDirectonToSimon();
					vx = (this->nx > 0) ?
						PANTHER_RUN_SPEED :
						-PANTHER_RUN_SPEED;
				}

				if (jumping)
					jumping = false;				

				y += DEFLECTION_AVOID_OVERLAPPING * ny;
				vy = 0;				
				groundStandingOn = e->obj;
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

CPanther::CPanther() : jumping(false), groundStandingOn(NULL)
{
}
