#include "PinkBat.h"


void CPinkBat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = BAT_BBOX_WIDTH + left;
	bottom = BAT_BBOX_HEIGHT + top;
}

void CPinkBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);

	// Update the position
	x += dx;
	y += dy;


	// Vertical movement
	if (vy > 0 && y > line + BAT_MOVEMENT_AMPLITUDE)
	{
		vy = -vy;
		y = line + BAT_MOVEMENT_AMPLITUDE;
	}
	
	if (vy < 0 && y < line - BAT_MOVEMENT_AMPLITUDE)
	{
		vy = -vy;
		y = line - BAT_MOVEMENT_AMPLITUDE;
	}


	// Horizontal movement 
	vx = nx * BAT_SPEED_VX;	


	// Get the animation matched with the state
	PickAnimation();


	// When go out of viewport
	if (this->IsInViewport() == false)
		SetState(STATE_INVISIBLE);
}

void CPinkBat::PickAnimation()
{
	if (vx != 0)
	{
		currentAniID = (nx > 0) ?
			(int)PinkBatAniID::FLYING_RIGHT :
			(int)PinkBatAniID::FLYING_LEFT;
	}
}

CPinkBat::CPinkBat()
{
	vx = BAT_SPEED_VX;
	vy = BAT_SPEED_VY;

	point = PINK_BAT_POINT_AWARD;
	health = PINK_BAT_DEFAULT_HEALTH;
	damage = PINK_BAT_DAMAGE;
}
