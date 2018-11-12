#pragma once
#include <Windows.h>
#include "GameObject.h"

class CTimer
{
	static CTimer * __instance;
	CTimer();

	bool freezing;
	DWORD freezeTime;
	DWORD startFreezeTime;
	vector<LPGAMEOBJECT> * objects;

public:

	void Update(WORD dt, vector<LPGAMEOBJECT> *objects);
	void Freeze(WORD time, LPGAMEOBJECT exceptedObj = NULL);

	static CTimer * GetInstance();
};

