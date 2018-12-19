#include "AutoMoveZone.h"



void CAutoMoveZone::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	right = x + width;
	top = y;
	bottom = top + height;
}

bool CAutoMoveZone::IsInViewport()
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
