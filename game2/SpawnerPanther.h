#pragma once
#include "MovableObject.h"

/*
	The class describes an invisible object.
	Panther monster will be spawned right at the position of this spawner.
*/
class CSpawnerPanther: public CActiveObject
{
	bool spawning = true;
	LPACTIVEOBJECT panther;		// the panther that has been spawned by this spawner

public:
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override { /* Nothing here */ }
	void Update(DWORD dt, vector<LPGAMEOBJECT> * coObjects) override;
	void SetupPantherInfo(float &x, float &y, int &nx);

	CSpawnerPanther();
};

