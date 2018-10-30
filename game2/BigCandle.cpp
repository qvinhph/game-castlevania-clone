#include "BigCandle.h"


void CBigCandle::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + TORCH_BBOX_WIDTH;
	bottom = y + TORCH_BBOX_HEIGHT;
}

CBigCandle::CBigCandle()
{
	currentAniID = (int)BigCandleAniID::IDLE;
}