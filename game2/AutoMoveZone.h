#pragma once
#include "GameObject.h"

class CAutoMoveZone : public CGameObject
{
	float width;
	float height;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override { /* Render nothing because this is an invisible object */ }

	CAutoMoveZone(float width, float height) : width(width), height(height) {}
};

