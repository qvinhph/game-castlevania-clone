#pragma once
#include "DestroyingFlame.h"

class CDestroyingFlames
{
	vector<CDestroyingFlame *> flames;
	static CDestroyingFlames * __instance;
	CDestroyingFlames() = default;
public:

	void Add(CDestroyingFlame * flame);
	void ShowAFlame(float central_x, float central_y);

	static CDestroyingFlames * GetInstance();
};

