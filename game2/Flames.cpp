#include "Flames.h"

CFlames * CFlames::__instance;

void CFlames::Add(CFlame * flame)
{
	flame->SetState(STATE_INVISIBLE);
	flames.push_back(flame);
}

/*
	Show a flame at the center position of the destroyed object given.
*/
void CFlames::ShowAFlame(LPGAMEOBJECT obj)
{
	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);
	float xCentral = (l + r) / 2;
	float yCentral = (t + b) / 2;

	CFlame* flame;

	for (auto it = flames.begin(); it != flames.end(); ++it)
	{
		flame = *it;
		if (flame->GetState() == STATE_INVISIBLE)
		{
			flame->SetState(STATE_VISIBLE);
			flame->StartToBurn();

			// Pass the item the object is holding to flame, if has
			// To drop it when flame runs out of burning time
			flame->SetHoldingItem(obj->GetHoldingItem());		

			// Calibrate the flame position to appear at the central of the objects
			flame->SetPosition(
				xCentral - FLAME_BBOX_WIDTH/2, 
				yCentral - FLAME_BBOX_HEIGHT/2);

			break;
		}
	}
}

CFlames * CFlames::GetInstance()
{
	if (__instance == NULL)
		__instance = new CFlames();

	return __instance;
}
