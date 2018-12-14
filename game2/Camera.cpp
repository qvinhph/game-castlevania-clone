#include "Camera.h"

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

void CCamera::NextLimitBound()
{
}

void CCamera::BackLimitBound()
{
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

CCamera * CCamera::GetInstance()
{
	if (__instance == NULL)
		__instance = new CCamera();

	return __instance;
}


CCamera::~CCamera()
{
}
