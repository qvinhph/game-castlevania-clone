#pragma once
#include "GameObject.h"

#define BURNING_TIME					300

#define FLAME_BBOX_HEIGHT				30
#define FLAME_BBOX_WIDTH				16		

enum class FlameAniID
{
	IDLE = 1400
};

/*
	The flame which appears after monsters/ destroyed
*/
class CFlame : public CGameObject
{
	DWORD burnStartTime;
public:

	void Render() override;
	void Destroy() override;

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void StartToBurn();

	CFlame();
};

