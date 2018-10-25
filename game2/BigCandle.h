#pragma once
#include "GameObject.h"

#define TORCH_BBOX_HEIGHT 32
#define TORCH_BBOX_WIDTH 16

enum class BigCandleAniID
{
	IDLE = 1200
};

class CBigCandle : public CGameObject
{

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override;

};

