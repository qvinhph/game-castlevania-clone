#include "InvisibleWater.h"

void CInvisibleWater::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;
}

CInvisibleWater::CInvisibleWater(float width, float height) : width(width), height(height)
{
}
