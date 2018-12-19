#pragma once
#include "Rock.h"

#define ROCK1_INITIAL_SPEED_X		 -0.1f
#define ROCK1_INITIAL_SPEED_Y		 -0.2f

#define ROCK2_INITIAL_SPEED_X		 0.01f
#define ROCK2_INITIAL_SPEED_Y		 -0.3f

#define ROCK3_INITIAL_SPEED_X		 0.06f
#define ROCK3_INITIAL_SPEED_Y		 -0.25f

#define ROCK4_INITIAL_SPEED_X		 0.07f
#define ROCK4_INITIAL_SPEED_Y		 -0.2f

class CRocks
{
	static CRocks * __instance;
	vector<CRock *> rocks;

public:

	void Add(CRock * rock);
	void ShowSomeRocksAfterBreaking(float x, float y);

	static CRocks * GetInstance();

};

