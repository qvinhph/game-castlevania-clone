#include "Bubble.h"



void CBubble::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + BUBBLE_BBOX_WIDTH;
	bottom = y + BUBBLE_BBOX_HEIGHT;
}

void CBubble::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);

	// Fall down
	vy += BUBBLE_FALL_GRAVITY * dt;

	// Pick animation
	currentAniID = (int)BubbleAniID::IDLE;

	// Update position
	x += dx;
	y += dy;

	// Re-set the state
	if (this->IsInViewport() == false)
		this->SetState(STATE_INVISIBLE);
}
