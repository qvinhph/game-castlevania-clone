#include "SecretBrick.h"
#include "Rocks.h"
#include "Items.h"

DWORD CSecretBrick::cannot_be_hit_start = TIMER_IDLE;

void CSecretBrick::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + SECRECT_BRICK_BBOX_WIDTH;
	bottom = y + SECRECT_BRICK_BBOX_HEIGHT;
}

void CSecretBrick::Destroy()
{
	CRocks::GetInstance()->ShowSomeRocksAfterBreaking(this->x, this->y);
	CItems::GetInstance()->CheckAndDrop(this);
	this->SetState(STATE_INVISIBLE);
}

void CSecretBrick::BeHit(int damage)
{
	if (GetTickCount() - cannot_be_hit_start > CANNOT_BE_HIT_TIME)
		cannot_be_hit_start = TIMER_IDLE;

	if (cannot_be_hit_start == TIMER_IDLE)
	{
		CGameObject::BeHit(damage);
		cannot_be_hit_start = GetTickCount();
	}
}

CSecretBrick::CSecretBrick(int currentAniID)
{
	this->currentAniID = currentAniID;
}
