#include "Timer.h"


CTimer * CTimer::__instance = NULL;

void CTimer::Update(WORD dt, vector<LPGAMEOBJECT>* objects)
{
	if (freezing)
	{
		if (GetTickCount() - startFreezeTime > freezeTime)
		{
			for (UINT i = 0; i < this->objects->size(); ++i)
				this->objects->at(i)->SetFreezing(false);

			this->freezing = false;
		}
		else
			for (UINT i = 0; i < this->objects->size(); ++i)
				this->objects->at(i)->FreezeAnimation();
	}
	else
		this->objects = objects;			
}

void CTimer::Freeze(WORD time, LPGAMEOBJECT exceptedObj)
{
	startFreezeTime = GetTickCount();
	freezeTime = time;
	freezing = true;

	for (UINT i = 0; i < objects->size(); ++i)
		objects->at(i)->SetFreezing(true);

	// Exception
	if (exceptedObj != NULL)
		exceptedObj->SetFreezing(false);
}

CTimer * CTimer::GetInstance()
{
	if (__instance == NULL)
		__instance = new CTimer();

	return __instance;
}

CTimer::CTimer()
{
	freezing = false;
}



