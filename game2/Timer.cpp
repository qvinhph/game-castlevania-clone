#include "Timer.h"
#include "Board.h"


CTimer * CTimer::__instance = NULL;

void CTimer::Update(WORD dt, vector<LPGAMEOBJECT>* objects)
{
	TimeCount(dt);

	if (freezing)
	{
		// If run out of freezing time
		if (GetTickCount() - startFreezeTime > freezeTime)
		{
			for (UINT i = 0; i < this->objects->size(); ++i)
				this->objects->at(i)->SetFreezing(false);

			this->freezing = false;
		}

		// Else, keep freezing the objects' animation
		else
			for (UINT i = 0; i < this->objects->size(); ++i)
				if (this->objects->at(i)->GetFreezing() == true)
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

void CTimer::TimeCount(DWORD dt)
{
	DWORD now = GetTickCount();
	if (now - unit_of_time_start > TICK_PER_UNIT_TIME)
	{
		CBoard::GetInstance()->AddTime(-1);
		unit_of_time_start = GetTickCount();
	}
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
	unit_of_time_start = GetTickCount();
}



