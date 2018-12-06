#pragma once
#include "GameObject.h"

class CInvisibleWall : public CGameObject
{

	float width;
	float height;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override { /* nothing here */ };
	bool IsInViewport() override;

	CInvisibleWall(float width, float height);
};

