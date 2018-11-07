#include "Flame.h"
#include "Items.h"

void CFlame::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	// Empty. We dont need this.
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


