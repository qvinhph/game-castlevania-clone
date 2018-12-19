#include "Rock.h"



void CRock::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + ROCK_BBOX_WIDTH;
	bottom = y + ROCK_BBOX_HEIGHT;
}

void CRock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);

	// Fall down
	vy += ROCK_FALL_GRAVITY * dt;

	// Pick animation
	currentAniID = (int)RockAniID::IDLE;

	// Update position
	x += dx;
	y += dy;

	// Re-set the state
	if (this->IsInViewport() == false)
		this->SetState(STATE_INVISIBLE);
}
