#pragma once
#include "Flame.h"
#include "GameObject.h"

/*
	The class helps managing the Flame:
*/
class CFlames
{
	vector<CFlame *> flames;

	static CFlames * __instance;
	CFlames() = default;

public:

	void Add(CFlame * flame);
	void ShowAFlame(LPGAMEOBJECT obj);

	static CFlames * GetInstance();
};

