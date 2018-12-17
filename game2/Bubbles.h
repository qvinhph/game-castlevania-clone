#pragma once
#include "Bubble.h"

#define BUBBLE1_INITIAL_SPEED_X		 0.02
#define BUBBLE1_INITIAL_SPEED_Y		 -0.35
				
#define BUBBLE2_INITIAL_SPEED_X		 -0.05f
#define BUBBLE2_INITIAL_SPEED_Y		 -0.2f
									 
#define BUBBLE3_INITIAL_SPEED_X		 0.05f
#define BUBBLE3_INITIAL_SPEED_Y		 -0.2f

class CBubbles
{

	static CBubbles * __instance;
	vector<CBubble *> bubbles;

public:
	
	void Add(CBubble * bubble);
	void ShowSomeBubblesForFish(float x, float y);

	static CBubbles * GetInstance();
};

