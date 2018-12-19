#pragma once
#include "MovableObject.h"

#define ZOMBIE_SPAWNER_REST_TIME		500			// Rest time before spawn another zombie
#define ZOMBIE_SPAWNER_PAUSE_TIME		5000		// Pause time before another spawn-season starts
#define NUMBER_OF_ZOMBIES				3			// Number of zombies will be spawn in a single spawn-season
#define SAFE_SPACE_TO_SPAWN				96			// Minimum distance from Simon to spawn a zombie

/*
	The class describes an invisible object.
	When Simon overlaps on this, Zombie monster will be spawned by somehow.
*/
class CSpawnerZombie: public CActiveObject
{
	// The number of zombie that has already spawned in the spawn-season
	static int spawnCounter;	

	// Timers, also be used as FLAG for the action when assigned to TIMER_IDLE(false) or non-TIMER_IDLE(true)
	// Assigned to TIMER_IDLE			: the timer is not working,
	// Assigned to TIMER_ETERNAL		: the timer is working and the action is not going to stop
	// Assigned to unsigned number		: the timer is working
	static DWORD rest_start;
	static DWORD pause_start;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override { /* Nothing here */ }
	void Update(DWORD dt, vector<LPGAMEOBJECT> * coObjects) override;

	bool IsFarEnoughToSpawn();
	void SetupZombieInfo(float &x, float &y, int &nx);
};

