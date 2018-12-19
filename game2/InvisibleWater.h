#pragma once 
#include "GameObject.h"

class CInvisibleWater : public CGameObject
{

	float width;
	float height;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override { /* nothing here */ };
	CInvisibleWater(float width, float height);
};

