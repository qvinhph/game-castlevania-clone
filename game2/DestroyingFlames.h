#pragma once
#include "DestroyingFlame.h"

/*
	The class helps managing the DestroyingFlame:
*/
class CDestroyingFlames
{
	vector<CDestroyingFlame *> flames;

	static CDestroyingFlames * __instance;
	CDestroyingFlames() = default;

public:

	void Add(CDestroyingFlame * flame);
	void ShowAFlame(float x, float y);

	static CDestroyingFlames * GetInstance();
};

