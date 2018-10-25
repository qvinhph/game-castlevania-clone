#pragma once
#include "GameObject.h"

#define FIRE_SHOWING_TIME 150

enum class DesFireAniID
{
	IDLE = 1400
};

// The fire after monsters/ breakable objects destroying.
class CDestroyedFire : public CGameObject
{
	DWORD fireStartTime;
	bool visible;

public:

	void Render() override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

	void ShowThenHide(CGameObject * obj);
};

