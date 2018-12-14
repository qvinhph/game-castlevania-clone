#include "InvisibleWall.h"

void CInvisibleWall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + this->width;
	bottom = y + this->height;
}

bool CInvisibleWall::IsInViewport()
{
	// The viewport bounding box
	float vpLeft, vpTop, vpRight, vpBottom;
	cameraInstance->GetBoundingBox(vpLeft, vpTop, vpRight, vpBottom);
	
	// The object bounding box
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);

	if (vpLeft > right || vpTop > bottom 
		|| vpRight < left || vpBottom < top)
		return false;

	return true;
}

CInvisibleWall::CInvisibleWall(float width, float height) : width(width), height(height)
{
}
