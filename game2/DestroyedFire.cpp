#include "DestroyedFire.h"



void CDestroyedFire::Render()
{
	int aniID = (int)DesFireAniID::IDLE;
	animations->Get(aniID)->Render(x, y);
}

void CDestroyedFire::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	// Empty. We dont need this.
}


void CDestroyedFire::ShowThenHide(CGameObject * obj)
{
	// find the center position of the object
	float left, top, right, bottom;
	obj->GetBoundingBox(left, top, right, bottom);

	float posX = (left + right) / 2;
	float posY = (top + bottom) / 2;
	
}

