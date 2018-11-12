#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprite.h"
#include "Sprites.h"

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
	void Render(float x, float y, ARGB &argb = ARGB());

	int GetCurrentFrame() { return currentFrame; };
	void SetCurrentFrame(int index) { this->currentFrame = index; }

	void ResetFrameStartTime();
};

typedef CAnimation * LPANIMATION;

