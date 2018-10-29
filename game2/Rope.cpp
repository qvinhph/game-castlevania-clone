#include "Rope.h"
#include "Simon.h"
#include "GameObject.h"
#include "BigCandle.h"
#include "debug.h"


CRope * CRope::__instance = NULL;

void CRope::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Do nothing if the rope isn't used
	if (!visible)
		return;
	CMovableObject::Update(dt);

	SetMatchedAnimation(state);
	UpdateRopePosition(currentAniID);
	
	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() != 0)
		ProceedCollisions(coEvents);

	// Check overlapping
	if (animations->Get(currentAniID)->GetCurrentFrame() == 2)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			float left, top, right, bottom;
			float leftObj, topObj, rightObj, bottomObj;

			LPGAMEOBJECT obj = (coObjects->at(i));

			if (dynamic_cast<CBigCandle *>(obj))
			{
				CBigCandle * candle = dynamic_cast<CBigCandle *>(obj);
				candle->GetBoundingBox(leftObj, topObj, rightObj, bottomObj);
				this->GetBoundingBox(left, top, right, bottom);

				// DebugOut(L"\nleft: %f - top: %f - right: %f - bottom: %f", left, top, right, bottomObj);
				// DebugOut(L"\nleft: %f - top: %f - right: %f - bottom: %f\n\n", leftObj, topObj, rightObj, bottomObj);

				if (left < rightObj && right > leftObj &&
					top < bottomObj && bottom > topObj)
				{
					candle->Destroy();
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CRope::Render()
{
	// Just render when Simon uses the rope to attack
	if (visible)
	{
		animations->Get(currentAniID)->Render(x, y);
		RenderBoundingBox();
	}
}

/*
	Get the current animation match with the state of the rope
*/
void CRope::SetMatchedAnimation(int state)
{
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
}

void CRope::ProceedCollisions(std::vector<LPCOLLISIONEVENT> &coEvents)
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
			right = x + FRONT_ROPE_LV3_BBOX_WIDTH;
			bottom = y + FRONT_ROPE_LV3_BBOX_HEIGHT;
			break;
		}

		left = x;
		top = y;

		//DebugOut(L"Succeed getting the rope bbox\n");
	}
}

/*
	Update rope position to match every frame with simon 
	in the specified animation.
*/
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

/*
	To show or hide the rope
*/
void CRope::SetVisible(bool visible)
{
	this->visible = visible;
	
	// hide the rope
	if (!visible)
	{
		SetPosition(-100, -100);

		// To rearrange the rope frames
		if (currentAniID > 0)
		{
			//DebugOut(L"\n rope reset: %d", currentAniID);
			this->ResetAnimation(currentAniID);
		}
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
}

