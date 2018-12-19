#include "InvisibleWall.h"

void CInvisibleWall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + this->width;
	bottom = y + this->height;
}

CInvisibleWall::CInvisibleWall(float width, float height) : width(width), height(height)
{
}
