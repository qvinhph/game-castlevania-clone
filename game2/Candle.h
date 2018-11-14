#pragma once
#include "GameObject.h"

#define CANDLE_BBOX_HEIGHT		32.0f
#define CANDLE_BBOX_WIDTH		16.0f

enum class CandleAniID
{
	IDLE = 1300
};

class CCandle : public CGameObject
{
public:
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

	CCandle() { this->currentAniID = (int)CandleAniID::IDLE; }
};

