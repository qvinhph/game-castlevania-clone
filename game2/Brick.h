#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  32
#define BRICK_BBOX_HEIGHT 32

enum BrickAniID
{
	IDLE = 1100
};

class CBrick : public CGameObject
{
	static unordered_map<int, LPANIMATION> brickAnimations;
public:
	void AddAnimation(int aniID) override;
	void Render() override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
};

