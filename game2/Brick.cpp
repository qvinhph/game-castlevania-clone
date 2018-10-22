#include "Brick.h"


void CBrick::Render()
{
	RenderAnimation(BrickAniID::IDLE);
}

void CBrick::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BRICK_BBOX_WIDTH;
	bottom = y + BRICK_BBOX_HEIGHT;
}
