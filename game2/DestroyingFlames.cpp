#include "DestroyingFlames.h"

CDestroyingFlames * CDestroyingFlames::__instance;

void CDestroyingFlames::Add(CDestroyingFlame * flame)
{
	flames.push_back(flame);
}

void CDestroyingFlames::ShowAFlame(float central_x, float central_y)
{
	for (UINT i = 0; i < flames.size(); i++)
	{
		if (flames[i]->GetState() == STATE_INVISIBLE)
		{
			flames[i]->SetState(STATE_VISIBLE);
			flames[i]->StartToBurn();

			// Calibrate the flame position to appear at the central of the objects
			flames[i]->SetPosition(
				central_x - DESTROYING_FLAME_BBOX_WIDTH/2, 
				central_y - DESTROYING_FLAME_BBOX_HEIGHT/2);

			break;
		}
	}
}

CDestroyingFlames * CDestroyingFlames::GetInstance()
{
	if (__instance == NULL)
		__instance = new CDestroyingFlames();

	return __instance;
}
