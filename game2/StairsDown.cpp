#include "StairsDown.h"


void CStairsDown::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + STAIRS_DOWN_BBOX_WIDTH;
	bottom = y + STAIRS_DOWN_BBOX_HEIGHT;
}
