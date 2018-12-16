#pragma once 
#include "MovableObject.h"
#include "Fish.h"

#define FISH_SPAWNER_PAUSE_TIME					8000
#define SUITABLE_DISTANCE_FROM_SIMON_TO_SPAWN	80

class CSpawnerFish : public CActiveObject
{
	DWORD spawning_pause_start = TIMER_IDLE;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> * coObjects = NULL) override;
	void Render() override { /* Render nothing */ }

	void SetupFish(LPGAMEOBJECT const &fish);
	void SetupFish(CFish * const &fish);
	bool IsReadyToSpawn();

};

