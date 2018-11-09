#pragma once
#include "MovableObject.h"

#define DAGGER_BBOX_HEIGHT		18.0f
#define DAGGER_BBOX_WIDTH		32.0f
#define DAGGER_SPEED_X			0.5f

enum class ItemDaggerAniID
{
	IDLE = 1800
};

class CItemDagger : public CMovableObject
{

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) override;

	CItemDagger() { this->currentAniID = (int)ItemDaggerAniID::IDLE; }
};

