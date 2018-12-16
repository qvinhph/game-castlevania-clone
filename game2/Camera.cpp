#include "Camera.h"
#include "debug.h"

CCamera * CCamera::__instance = NULL;

void CCamera::AddLimitBound(float left, float top, float right, float bottom)
{
	RECT limitBound;
	limitBound.left = left;
	limitBound.right = right;
	limitBound.top = top;
	limitBound.bottom = bottom;

	limitBounds.push_back(limitBound);
}

void CCamera::GetLimitBound(float & left, float & top, float & right, float & bottom)
{
	RECT bound = limitBounds[currentLimitBound];

	left = bound.left;
	top = bound.top;
	right = bound.right;
	bottom = bound.bottom;
}

void CCamera::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + viewportWidth;
	bottom = y + viewportHeight;
}

void CCamera::ChangeLimitBound(float x, float y)
{
	//float left, top, right, bottom;
	RECT bound;
	//obj->GetBoundingBox(left, top, right, bottom);

	for (UINT i = 0; i < limitBounds.size(); i++)
	{
		bound = limitBounds[i];

		/*if (left > bound.right || right < bound.left ||
			top > bound.bottom || bottom < bound.top)
			continue;*/

		if (x < bound.right && x > bound.left
			&& y < bound.bottom && y > bound.top)
		{
			this->currentLimitBound = i;
			return;
		}
	}

	DebugOut(L"\n[ERROR] Cannot focus the camera to the given point");
}

CCamera * CCamera::GetInstance()
{
	if (__instance == NULL)
		__instance = new CCamera();

	return __instance;
}


CCamera::~CCamera()
{
}
