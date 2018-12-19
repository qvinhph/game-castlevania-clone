#include "Gate.h"



void CGate::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + GATE_BBOX_WIDTH;
	bottom = top + GATE_BBOX_HEIGHT;
}

void CGate::Render()
{
	currentAniID = (closing) ?
		(int)GateAniID::CLOSING :
		(int)GateAniID::OPENING;

	CGameObject::Render();
}

