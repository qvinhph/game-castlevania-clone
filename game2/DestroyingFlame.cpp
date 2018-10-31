#include "DestroyingFlame.h"

void CDestroyingFlame::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	// Empty. We dont need this.
}

void CDestroyingFlame::Render()
{
	if (state == STATE_VISIBLE)
	{
		if (GetTickCount() - burnStartTime > BURNING_TIME)
		{
			SetState(STATE_INVISIBLE);
			ResetAnimation(currentAniID);
		}
		else
			CGameObject::Render();
	}
}

void CDestroyingFlame::StartToBurn()
{
	burnStartTime = GetTickCount();
}

CDestroyingFlame::CDestroyingFlame()
{
	currentAniID = (int)DestroyingFlameAniID::IDLE;
}


