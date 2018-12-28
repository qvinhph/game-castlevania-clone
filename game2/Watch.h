#pragma once
#include "MovableObject.h"

#define WATCH_BBOX_WIDTH		30
#define WATCH_BBOX_HEIGHT		32

#define WATCH_HEART_COST		5

enum class WatchAniID
{
	IDLE = 3000,
};

class CWatch : public CActiveObject
{
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

};

