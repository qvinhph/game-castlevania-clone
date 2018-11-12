#include "Flame.h"
#include "Items.h"

void CFlame::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + FLAME_BBOX_WIDTH;
	bottom = top + FLAME_BBOX_HEIGHT;
}

void CFlame::Render()
{
	if (state == STATE_VISIBLE)
	{
		if (GetTickCount() - burnStartTime > BURNING_TIME)
		{
			CItems::GetInstance()->CheckAndDrop(this);
			SetState(STATE_INVISIBLE);

			ResetAnimation(currentAniID);
		}
		else
			CGameObject::Render();
	}	
}

void CFlame::StartToBurn()
{
	burnStartTime = GetTickCount();
}

CFlame::CFlame()
{
	currentAniID = (int)FlameAniID::IDLE;
}


