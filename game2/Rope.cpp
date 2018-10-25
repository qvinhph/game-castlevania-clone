#include "Rope.h"
#include "Simon.h"
#include "GameObject.h"
#include "BigCandle.h"
#include "debug.h"


CRope * CRope::__instance = NULL;

void CRope::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// We dont need to do this function 
	// if the rope isn't used.
	if (!visible)
		return;

	CSimon::GetInstance()->GetSpeed(vx, vy);
	CMovableObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() != 0)
		ProcessCollision(coEvents);

	// Check overlapping
	//for (UINT i = 0; i < coObjects->size(); i++)
	//{
	//	float l, t, r, b;
	//	float lo, to, ro, bo;
	//	
	//	if (animations->Get(currentAniID)->GetCurrentFrame() == 2)	// EX1
	//	{
	//		LPGAMEOBJECT ptr = (coObjects->at(i));
	//
	//		if (dynamic_cast<CBigCandle *>(ptr))
	//		{
	//			
	//			CBigCandle * candle = dynamic_cast<CBigCandle *>(ptr);
	//			candle->GetBoundingBox(lo, to, ro, bo);
	//			this->GetBoundingBox(l, t, r, b);
	//			DebugOut(L"\nleft: %f - top: %f - right: %f - bottom: %f", l, t, r, b);
	//			DebugOut(L"\nleft: %f - top: %f - right: %f - bottom: %f\n\n", lo, to, ro, bo);
	//			if (l < ro && r > lo &&
	//				t > bo && b < to)
	//			{
	//				candle->Destroy();
	//			}
	//		}
	//	}		
	//}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
			(int)RopeAniID::LEVEL_ONE_RIGHT :
			(int)RopeAniID::LEVEL_ONE_LEFT;
		break;

	case ROPE_STATE_LEVEL2:
		currentAniID = (nx > 0) ?
			(int)RopeAniID::LEVEL_TWO_RIGHT :
			(int)RopeAniID::LEVEL_TWO_LEFT;
		break;

	default:
		currentAniID = (nx > 0) ?
			(int)RopeAniID::LEVEL_THREE_RIGHT :
			(int)RopeAniID::LEVEL_THREE_LEFT;
		break;
	}

	RenderAnimation(currentAniID);
	RenderBoundingBox();
}

void CRope::ProcessCollision(std::vector<LPCOLLISIONEVENT> &coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;

	float min_tx, min_ty, nx, ny;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];

		// Collision logic with candles
		if (dynamic_cast<CBigCandle *>(e->obj))
		{
			CBigCandle * candle = dynamic_cast<CBigCandle *>(e->obj);
			candle->Destroy();
		}
	}

}

void CRope::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	// Just get the only rope in the front of simon
	if ((nx > 0 && x > xS) || (nx < 0 && x < xS))
	{
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

		//DebugOut(L"Succeed getting the rope bbox\n");
	}
	/*else
	{
		DebugOut(L"Not get the rope bbox yet\n");
	}*/
}

/*
	Because the rope may has different position each frame,
	we need modify this function
*/
void CRope::RenderAnimation(int aniID)
{
	UpdateRopePosition(aniID);

	CGameObject::RenderAnimation(aniID);
}

void CRope::UpdateRopePosition(int aniID)
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

	// the third frame of Rope Animation is the one we want to treat differently.
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

	// EX1
	//currentAniID = (int)RopeAniID::LEVEL_ONE_RIGHT;
}

