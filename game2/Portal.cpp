#include "Portal.h"



void CPortal::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;
}

bool CPortal::IsInViewport()
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
