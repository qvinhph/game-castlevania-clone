#include "Brick.h"

unordered_map<int, LPANIMATION> CBrick::brickAnimations;

void CBrick::AddAnimation(int aniID)
{
	brickAnimations[aniID] = CAnimations::GetInstance()->Get(aniID);
}

void CBrick::Render()
{
	brickAnimations[BrickAniID::IDLE]->Render(x, y);
}

void CBrick::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BRICK_BBOX_WIDTH;
	bottom = y + BRICK_BBOX_HEIGHT;
}
