#include "InvisibleWall.h"

void CInvisibleWall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + this->width;
	bottom = y + this->height;
}

void CInvisibleWall::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;
}
