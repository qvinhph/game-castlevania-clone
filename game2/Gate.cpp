#include "Gate.h"



void CGate::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (closing == true)
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
	currentAniID = (closing) ?
		(int)GateAniID::CLOSING :
		(int)GateAniID::OPENING;

	CGameObject::Render();
}

