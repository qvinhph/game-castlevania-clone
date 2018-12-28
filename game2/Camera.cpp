#include "Camera.h"
#include "debug.h"
#include "Textures.h"
#include "Simon.h"

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
	RECT bound;

	for (UINT i = 0; i < limitBounds.size(); i++)
	{
		bound = limitBounds[i];

		if (x < bound.right && x > bound.left
			&& y < bound.bottom && y > bound.top)
		{
			this->currentLimitBound = i;
			return;
		}
	}

	DebugOut(L"\n[ERROR] Cannot focus the camera to the given point");
}

void CCamera::FocusOnPoint(float x, float y)
{
	if (slowlyMoveEffect == true)
	{
		SlowlyMoveViewport();
		return;
	}		

	if (lockEffect == false)
	{
		// Calculate the camera position that is supposed to be
		float xCam = x - viewportWidth / 2;
		float yCam = y - viewportHeight / 2;

		// Change the limit bound depend on the given point
		ChangeLimitBound(x, y);


		// Get the limit bound
		RECT bound = limitBounds[currentLimitBound];


		// Limit the camera position
		if (xCam + viewportWidth > bound.right)
			xCam = bound.right - viewportWidth;
		if (xCam < bound.left)
			xCam = bound.left;

		if (yCam < bound.top)
			yCam = bound.top;
		if (yCam + viewportHeight > bound.bottom)
			yCam = bound.bottom - viewportHeight;

		// Set camera's position
		SetPosition(xCam, yCam);
	}
}

void CCamera::SlowlyMoveViewport()
{
	if (slowlyMoveEffect == true)
	{
		if (x < x_destination) x += SLOWLY_MOVE_PIXEL_PER_FRAME;
		if (y < y_destination) y += SLOWLY_MOVE_PIXEL_PER_FRAME;

		if (x >= x_destination && y >= y_destination)
			slowlyMoveEffect = false;
	}
}

void CCamera::StartSlowlyMoveViewport(float x_destination, float y_destination)
{
	this->x_destination = x_destination;
	this->y_destination = y_destination;
	slowlyMoveEffect = true;
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
