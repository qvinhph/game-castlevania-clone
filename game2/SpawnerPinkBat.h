#pragma once
#include "MovableObject.h"

#define ZOMBIE_SPAWNER_PAUSE_TIME			3000

/*
	The class describes an invisible object.
	When Simon overlaps on this, PinkBat monster will be spawned by somehow.
*/
class CSpawnerPinkBat: public CActiveObject
{
	DWORD spawn_pause;
public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override { /* Nothing here */ }
	void Update(DWORD dt, vector<LPGAMEOBJECT> * coObjects) override;

	void SetupBatInfo(float &x, float &y, int &nx, float & line);
};

