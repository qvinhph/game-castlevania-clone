#pragma once
#include "MovableObject.h"

#define ITEM_ROPE_BBOX_WIDTH 32
#define ITEM_ROPE_BBOX_HEIGHT 32

enum class ItemRopeAniID
{
	IDLE = 1400
};

/*
	The object when Simon touch will upgrade his Rope
*/
class CItemRope : public CActiveObject
{
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

};

