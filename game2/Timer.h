#pragma once
#include <Windows.h>
#include "GameObject.h"

#define TICK_PER_UNIT_TIME		1000

class CTimer
{
	static CTimer * __instance;
	CTimer();

	// For counting time
	DWORD unit_of_time_start;

	// For freezing
	bool freezing;
	DWORD freezeTime;
	DWORD startFreezeTime;
	vector<LPGAMEOBJECT> objects;

public:

	void Update(WORD dt, vector<LPGAMEOBJECT> *objects);
	void Freeze(WORD time, LPGAMEOBJECT exceptedObj = NULL);
	void TimeCount(DWORD dt);


	static CTimer * GetInstance();
};

