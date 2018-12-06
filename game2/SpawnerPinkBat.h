#pragma once
#include "MovableObject.h"

#define PAUSE_TIME			3000

/*
	The class describes an invisible object.
	When Simon overlaps on this, PinkBat monster will be spawned by somehow.
*/
class CSpawnerPinkBat: public CActiveObject
{
	float width;
	float height;

	DWORD spawn_pause;
public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override { /* Nothing here */ }
	void Update(DWORD dt, vector<LPGAMEOBJECT> * coObjects) override;

	void SetupBatInfo(float &x, float &y, int &nx, float & line);

	CSpawnerPinkBat(float width, float height);
};

