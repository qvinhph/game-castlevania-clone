#pragma once
#include "GameObject.h"

class CInvisibleWall : public CGameObject
{

	float width;
	float height;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override { /* nothing here */	};
	void SetSize(float width, float height);

};

