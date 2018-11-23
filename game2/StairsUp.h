#pragma once
#include "GameObject.h"

// The BBOX_HEIGHT must equal to the Simon's height for getting-out-the-on-stairs-function
#define STAIRS_UP_BBOX_HEIGHT			62
#define STAIRS_UP_BBOX_WIDTH			32 

// The class describes the invisible stairs-up start
class CStairsUp: public CGameObject
{
public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

	/*
		NOTES: currentAniID always equals -1
	*/
	void Render() override { /* nothing here */ };
};

