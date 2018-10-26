#pragma once
#include "MovableObject.h"

#define ITEM_ROPE_BBOX_WIDTH 16
#define ITEM_ROPE_BBOX_HEIGHT 16

enum class ItemRopeAniID
{
	IDLE = 1500
};

/*
	The object when Simon eats will upgrade his Rope
*/
class CItemRope : public CMovableObject
{
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);

};

