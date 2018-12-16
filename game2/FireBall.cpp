#include "FireBall.h"
#include "Weapons.h"

#define FIREBALL_POINT_AWARD		100
#define FIREBALL_DAMAGE				2
#define FIREBALL_DEFAULT_HEALTH		1


void CFireBall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + FIRE_BALL_BBOX_WIDTH;
	bottom = y+ FIRE_BALL_BBOX_HEIGHT;
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CActiveObject::Update(dt);

	// Fireball's horizontally movement
	vx = (nx > 0) ?
		FIRE_BALL_SPEED_VX :
		-FIRE_BALL_SPEED_VX;

	// Choose matched animation
	currentAniID = (nx > 0) ?
		FireBallAniID::FLYING_RIGHT :
		FireBallAniID::FLYING_LEFT;

	// If the dagger go out the viewport
	if (this->IsInViewport() == false)
		SetState(STATE_INVISIBLE);

	// Update the position
	x += dx;
	y += dy;
}

void CFireBall::SetState(int state)
{
	CGameObject::SetState(state);

	// Make the fireball object available again to use next time
	if (state == STATE_INVISIBLE)
		CWeapons::GetInstance()->AddToStock(Weapon::FIREBALL);
}

CFireBall::CFireBall()
{
	health = FIREBALL_DEFAULT_HEALTH;
	point = FIREBALL_POINT_AWARD;
	damage = FIREBALL_DAMAGE;
}
