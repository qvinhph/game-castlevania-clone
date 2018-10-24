#include "Rope.h"
#include "Simon.h"


CRope * CRope::__instance = NULL;

void CRope::LevelUp()
{
	switch (state)
	{
	case ROPE_STATE_LEVEL1:
		state = ROPE_STATE_LEVEL2;
		break;

	case ROPE_STATE_LEVEL2:
		state = ROPE_STATE_LEVEL3;
		
	default:
		break;
	}
}

void CRope::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!visible)
		return;	

	// Handling collision
}

void CRope::Render()
{
	// Just render when Simon uses the rope to attack
	if (!visible)
		return;
	
	switch (state)
	{
	case ROPE_STATE_LEVEL1:
		currentAniID = (nx > 0) ?
				RopeAniID::LEVEL_ONE_RIGHT :
				RopeAniID::LEVEL_ONE_LEFT;
		break;

	case ROPE_STATE_LEVEL2:
		currentAniID = (nx > 0) ?
			RopeAniID::LEVEL_TWO_RIGHT :
			RopeAniID::LEVEL_TWO_LEFT;
		break;

	default:
		currentAniID = (nx > 0) ?
			RopeAniID::LEVEL_THREE_RIGHT :
			RopeAniID::LEVEL_THREE_LEFT;
		break;
	}
	
	RenderAnimation(currentAniID);
	//RenderBoundingBox();
}

void CRope::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	// Just get the only rope in the front of simon
	if ( (nx > 0 && x < xS) || (nx < 0 && x > xS) )
		return;

	switch (state)
	{
	case ROPE_STATE_LEVEL1:
		right = x + FRONT_ROPE_LV1_BBOX_WIDTH;
		bottom = y + FRONT_ROPE_LV1_BBOX_HEIGHT;
		break; 

	case ROPE_STATE_LEVEL2:
		right = x + FRONT_ROPE_LV2_BBOX_WIDTH;
		bottom = y + FRONT_ROPE_LV2_BBOX_HEIGHT;
		break;

	default:
	//case ROPE_STATE_LEVEL3:
		right = x + FRONT_ROPE_LV3_BBOX_WIDTH;
		bottom = y + FRONT_ROPE_LV3_BBOX_HEIGHT;
		break;
	}
	
	left = x;
	top = y;
}

/*
	Because the rope may has different position each frame,
	we need modify this function
*/
void CRope::RenderAnimation(int aniID)
{
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	// get the current rope texture width
	int ropeTexWidth;
	switch (state)
	{
	case ROPE_STATE_LEVEL1:
		ropeTexWidth = FRONT_ROPE_LV1_BBOX_WIDTH;
		break;

	case ROPE_STATE_LEVEL2:
		ropeTexWidth = FRONT_ROPE_LV2_BBOX_WIDTH;
		break;

	case ROPE_STATE_LEVEL3:
		ropeTexWidth = FRONT_ROPE_LV3_BBOX_WIDTH;
		break;

	default:
		break;
	}

	LPANIMATION ani = CAnimations::GetInstance()->Get(aniID);

	// the third frame in CAnimation::frames(vector) is the one we want to treat differently.
	if (ani->GetCurrentFrame() == 2) 
	{
		x = (nx > 0) ?
			xS + SIMON_ATTACKING_BBOX_WIDTH :
			xS - ropeTexWidth;

		y = yS + Y_DISTANCE_FRONT_ROPE_AND_SIMON_POS;
	}
	else
	{
		x = (nx > 0) ?
			xS - BACK_ROPE_BBOX_WIDTH :
			xS + SIMON_ATTACKING_BBOX_WIDTH;

		y = yS + Y_DISTANCE_BACK_ROPE_AND_SIMON_POS;
	}

	CGameObject::RenderAnimation(aniID);
}

void CRope::SetVisible(bool visible)
{
	this->visible = visible;
	
	// hide the rope
	if (!visible)
	{
		SetPosition(-100, -100);

		// clean the animation to prevent
		if (currentAniID >= 0)
			this->ResetAnimation(currentAniID);
	}
		
	
}

CRope * CRope::GetInstance()
{
	if (__instance == NULL)
		__instance = new CRope();

	return __instance;
}

CRope::CRope()
{
	state = ROPE_STATE_LEVEL1;
	visible = false;
}

