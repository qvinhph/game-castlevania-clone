#pragma once
#include "GameObject.h"

#define TORCH_BBOX_HEIGHT 64
#define TORCH_BBOX_WIDTH 32

enum class BigCandleAniID
{
	IDLE = 1200
};

class CBigCandle : public CGameObject
{
public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	CBigCandle() { this->currentAniID = (int)BigCandleAniID::IDLE; }
};

