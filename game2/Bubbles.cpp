#include "Bubbles.h"


CBubbles * CBubbles::__instance = NULL;



void CBubbles::Add(CBubble * bubble)
{
	bubble->SetState(STATE_INVISIBLE);
	bubbles.push_back(bubble);
}

void CBubbles::ShowSomeBubblesForFish(float x, float y)
{
	CBubble * bubble = NULL;

	UINT i = 0;
	//// Setup for bubble no.1
	for (; i < bubbles.size(); i++)
	{
		if (bubbles[i]->GetState() == STATE_INVISIBLE)
		{
			bubble = bubbles[i];
			i++;
			break;
		}
	}
	if (bubble != NULL)
	{
		bubble->SetPosition(x, y);
		bubble->SetSpeed(BUBBLE1_INITIAL_SPEED_X, BUBBLE1_INITIAL_SPEED_Y);
		bubble->SetState(STATE_VISIBLE);
	}
	


	// Setup for bubble no.2
	for (; i < bubbles.size(); i++)
	{
		if (bubbles[i]->GetState() == STATE_INVISIBLE)
		{
			bubble = bubbles[i];
			i++;
			break;
		}
	}
	if (bubble != NULL)
	{
		bubble->SetPosition(x, y);
		bubble->SetSpeed(BUBBLE2_INITIAL_SPEED_X, BUBBLE2_INITIAL_SPEED_Y);
		bubble->SetState(STATE_VISIBLE);
	}


	// Setup for bubble no.3
	for (; i < bubbles.size(); i++)
	{
		if (bubbles[i]->GetState() == STATE_INVISIBLE)
		{
			bubble = bubbles[i];
			i++;
			break;
		}
	}
	if (bubble != NULL)
	{
		bubble->SetPosition(x, y);
		bubble->SetSpeed(BUBBLE3_INITIAL_SPEED_X, BUBBLE3_INITIAL_SPEED_Y);
		bubble->SetState(STATE_VISIBLE);
	}
}

CBubbles * CBubbles::GetInstance()
{
	if (__instance == NULL)
		__instance = new CBubbles();

	return __instance;
}
