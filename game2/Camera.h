#pragma once
#include <Windows.h>
#include <vector>

using namespace std;

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

public:

	void AddLimitBound(float left, float top, float right, float bottom);
	void ClearLimits() { this->limitBounds.clear(); }

	void SetPosition(float x, float y) { this->x = x; this->y = y; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }

	void SetViewportSize(float width, float height) { this->viewportWidth = width; this->viewportHeight = height; }
	void GetViewportSize(float &width, float &height) { width = this->viewportWidth; height = this->viewportHeight; }


	void GetLimitBound(float &left, float &top, float &right, float &bottom);
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);


	// Change the current camera limit bound to the bound contain the given object
	void ChangeLimitBound(float x, float y);

	static CCamera * GetInstance();
	~CCamera();
};

