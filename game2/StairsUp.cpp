#include "StairsUp.h"


void CStairsUp::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + STAIRS_UP_BBOX_WIDTH;
	bottom = y + STAIRS_UP_BBOX_HEIGHT;
}
