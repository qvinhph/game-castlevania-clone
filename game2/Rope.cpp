#include "Rope.h"
#include "Simon.h"
#include "GameObject.h"
#include "BigCandle.h"
#include "debug.h"


CRope * CRope::__instance = NULL;

void CRope::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == STATE_VISIBLE)
	{
		CMovableObject::Update(dt);

		SetMatchedAnimation();
		UpdateRopePosition(currentAniID);

		vector<LPCOLLISIONEVENT> coEvents;
		coEvents.clear();
		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() != 0)
			ProceedCollisions(coEvents);

		// Check overlapping
		if (animations->Get(currentAniID)->GetCurrentFrame() == 2)
			for (UINT i = 0; i < coObjects->size(); i++)
				if (dynamic_cast<CBigCandle *>(coObjects->at(i))
					&& this->IsOverlapping(coObjects->at(i)))
					coObjects->at(i)->Destroy();

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

bool CRope::IsOverlapping(LPGAMEOBJECT obj)
{
	bool result = false;

	float left, top, right, bottom;
	float leftObj, topObj, rightObj, bottomObj;

	obj->GetBoundingBox(leftObj, topObj, rightObj, bottomObj);
	GetBoundingBox(left, top, right, bottom);

	if (left < rightObj && right > leftObj &&
		top < bottomObj && bottom > topObj)
		result = true;

	return result;
}

void CRope::Render()
{
	if (state == STATE_VISIBLE)
	{
		if (flickering)
		{
			if (argb.blue != 0)
				argb.blue = 0;
			else if (argb.green != 0)
				argb.green = 0;
			else
				argb = ARGB();

			animations->Get(currentAniID)->Render(x, y, argb);
		}
		else
			animations->Get(currentAniID)->Render(x, y);
	}
}

/*
	Get the current animation match with the current rope
*/
void CRope::SetMatchedAnimation()
{
	switch (level)
	{
	case ROPE_LEVEL1:
		currentAniID = (nx > 0) ?
			(int)RopeAniID::LEVEL_ONE_RIGHT :
			(int)RopeAniID::LEVEL_ONE_LEFT;
		break;

	case ROPE_LEVEL2:
		currentAniID = (nx > 0) ?
			(int)RopeAniID::LEVEL_TWO_RIGHT :
			(int)RopeAniID::LEVEL_TWO_LEFT;
		break;

	default:
		flickering = true;
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
		switch (level)
		{
		case ROPE_LEVEL1:
			right = x + FRONT_ROPE_LV1_BBOX_WIDTH;
			bottom = y + FRONT_ROPE_LV1_BBOX_HEIGHT;
			break;

		case ROPE_LEVEL2:
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
	switch (level)
	{
	case ROPE_LEVEL1:
		ropeTexWidth = FRONT_ROPE_LV1_BBOX_WIDTH;
		break;

	case ROPE_LEVEL2:
		ropeTexWidth = FRONT_ROPE_LV2_BBOX_WIDTH;
		break;

	case ROPE_LEVEL3:
		ropeTexWidth = FRONT_ROPE_LV3_BBOX_WIDTH;
		break;

	default:
		break;
	}

	LPANIMATION ani = CAnimations::GetInstance()->Get(aniID);

	// the third frame of Rope Animation is the one we want to treat differently.
	if (ani->GetCurrentFrame() == 2)	// 2 because vector counts index from 0 (OPT: CAN THIS CONSTANT REMOVED?)
	{
		x = (nx > 0) ?
			xS + SIMON_IDLE_BBOX_WIDTH :
			xS - ropeTexWidth;

		y = yS + Y_DISTANCE_FRONT_ROPE_AND_SIMON_POS;
	}
	else
	{
		x = (nx > 0) ?
			xS - BACK_ROPE_BBOX_WIDTH :
			xS + SIMON_IDLE_BBOX_WIDTH;

		y = yS + Y_DISTANCE_BACK_ROPE_AND_SIMON_POS;
	}
}


void CRope::LevelUp()
{
	switch (level)
	{
	case ROPE_LEVEL1:
		level = ROPE_LEVEL2;
		break;

	case ROPE_LEVEL2:
		level = ROPE_LEVEL3;

	default:
		break;
	}
}

void CRope::SetState(int state)
{
	CGameObject::SetState(state);

	// hide the rope
	if (state == STATE_INVISIBLE)
	{
		// TODO: NEED CLEANING THIS
		SetPosition(-100, -100);

		// To rearrange the rope frames
		if (currentAniID > 0)
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
	state = STATE_INVISIBLE;
	level = ROPE_LEVEL1;
}

