#pragma once
#include "GameObject.h"

#define BURNING_TIME					300

#define DESTROYING_FLAME_BBOX_HEIGHT	30
#define DESTROYING_FLAME_BBOX_WIDTH		16		

enum class DestroyingFlameAniID
{
	IDLE = 1400
};

/*
	The fire after monsters/ breakable objects destroying.
*/
class CDestroyingFlame : public CGameObject
{
	DWORD burnStartTime;
public:

	void Render() override;

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void StartToBurn();

	CDestroyingFlame();
};

