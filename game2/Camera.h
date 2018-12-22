#pragma once
#include <Windows.h>
#include <vector>

#define SLOWLY_MOVE_PIXEL_PER_FRAME		3

using namespace std;

/*
	Manage the camera limit bounds of the map
*/
class CCamera
{
	static CCamera * __instance;
	CCamera() = default;

	float x;
	float y;
	float viewportWidth;
	float viewportHeight;

	int currentLimitBound = 0;
	vector<RECT> limitBounds;
	
	// Slowly move effect
	bool slowlyMoveEffect = false;
	bool lockEffect = false;
	
	// to save the point that camera going to move to
	float x_destination;			
	float y_destination;

public:

	bool GetLockEffect() { return this->lockEffect; }
	void SetLockEffect(bool lockEffect) { this->lockEffect = lockEffect; }

	bool GetSlowlyMoveEffect() { return this->slowlyMoveEffect; }
	void SetSlowlyMoveEffect(bool slowlyMoveEffect) { this->slowlyMoveEffect = slowlyMoveEffect; }

	void AddLimitBound(float left, float top, float right, float bottom);
	void ClearLimits() { this->limitBounds.clear(); }

	void SetPosition(float x, float y) { this->x = x; this->y = y; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }

	void SetViewportSize(float width, float height) { this->viewportWidth = width; this->viewportHeight = height; }
	void GetViewportSize(float &width, float &height) { width = this->viewportWidth; height = this->viewportHeight; }

	void GetLimitBound(float &left, float &top, float &right, float &bottom);
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	// Change the current camera limit bound to the bound contain the given point
	void ChangeLimitBound(float x, float y);

	// Make the given point to be center of the camera
	void Centerize(float x, float y);
	void SlowlyMoveViewport();
	void StartSlowlyMoveViewport(float x_destination, float y_destination);

	static CCamera * GetInstance();
	~CCamera();
};

