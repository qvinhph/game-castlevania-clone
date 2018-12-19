#include "AutoMoveZone.h"

void CAutoMoveZone::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	right = x + width;
	top = y;
	bottom = top + height;
}
