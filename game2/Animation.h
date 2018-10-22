#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "AnimationFrame.h"

using namespace std;

class CAnimation
{
	DWORD frameStartTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;

public:
	CAnimation(int defaultTime);
	void AddFrame(int spriteID, DWORD time = 0);
	void Render(float x, float y, int alpha = 255);

	int CurrentFrame() { return currentFrame; };
};

typedef CAnimation * LPANIMATION;

