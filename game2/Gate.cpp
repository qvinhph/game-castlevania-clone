#include "Gate.h"



void CGate::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (openning == false)
	{
		left = x;
		top = y;
		right = left + GATE_BBOX_WIDTH;
		bottom = top + GATE_BBOX_HEIGHT;
	}
	else
	{
		left = right = x;
		top = bottom = y;
	}
}

void CGate::Render()
{
	currentAniID = (openning) ?
		(int)GateAniID::OPENING :
		(int)GateAniID::CLOSING;


	if (openning)
		if (GetTickCount() - open_start > GATE_OPEN_TIME)
		{
			open_start = 0;
			openning = false;
		}

	CGameObject::Render();
}

