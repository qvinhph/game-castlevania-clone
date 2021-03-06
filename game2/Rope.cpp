#include "Rope.h"
#include "Simon.h"
#include "GameObject.h"
#include "BigCandle.h"
#include "Candle.h"
#include "Zombie.h"
#include "Panther.h"
#include "PinkBat.h"
#include "Fish.h"
#include "FireBall.h"
#include "BossBat.h"
#include "SecretBrick.h"
#include "debug.h"


CRope * CRope::__instance = NULL;

void CRope::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == STATE_VISIBLE)
	{
		CActiveObject::Update(dt);

		SetMatchedAnimation();
		UpdateRopePosition(currentAniID);
		PickDamage();

		vector<LPCOLLISIONEVENT> coEvents;
		coEvents.clear();
		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() != 0)
			ProceedCollisions(coEvents);

		// Check overlapping
		if (animations->Get(currentAniID)->GetCurrentFrame() == 2)
			for (UINT i = 0; i < coObjects->size(); i++)
				if (this->IsOverlapping(coObjects->at(i)))
					if (dynamic_cast<CBigCandle *>(coObjects->at(i)) ||
						dynamic_cast<CCandle *>(coObjects->at(i)) || 
						dynamic_cast<CZombie *>(coObjects->at(i)) || 
						dynamic_cast<CPanther *>(coObjects->at(i)) || 
						dynamic_cast<CPinkBat *>(coObjects->at(i)) ||
						dynamic_cast<CFish *>(coObjects->at(i)) ||
						dynamic_cast<CFireBall *>(coObjects->at(i)) ||
						dynamic_cast<CBossBat *>(coObjects->at(i)) ||
						dynamic_cast<CSecretBrick *>(coObjects->at(i)))
						coObjects->at(i)->BeHit(this->damage);

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}


void CRope::Render()
{
	if (flicker_start != 0)
	{
		if (argb.blue != 0)
			argb.blue = 0;
		else if (argb.green != 0)
			argb.green = 0;
		else
			argb = ARGB();
	}

	CGameObject::Render();
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
		flicker_start = 1;		
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
		// empty now
	}
}

void CRope::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	left = x;
	top = y;

	// When the rope is in front of Simon.
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
	}
	else
	{
		right = x + BACK_ROPE_BBOX_WIDTH;
		bottom = y + BACK_ROPE_BBOX_HEIGHT;
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

	// Get the current rope texture width
	// Because different rope level may has different length
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

		y = yS + Y_DISTANCE_FRONTROPE_LOWER_SIMON;
	}
	else
	{
		x = (nx > 0) ?
			xS - BACK_ROPE_BBOX_WIDTH :
			xS + SIMON_IDLE_BBOX_WIDTH;

		y = yS;
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

void CRope::PickDamage()
{
	if (level == ROPE_LEVEL1)
		damage = ROPE_DAMAGE_LEVEL1;
	else
		damage = ROPE_DAMAGE_LEVEL2;
}

void CRope::SetState(int state)
{
	CGameObject::SetState(state);
	
	// Reset the current animation for next usage
	if (state == STATE_INVISIBLE)
		if (currentAniID > 0)
			this->ResetAnimationTimer(currentAniID);
}

bool CRope::IsOverlapping(LPGAMEOBJECT obj)
{
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	// When the rope is at the back of Simon, ignore it
	if ((nx > 0 && x < xS) || (nx < 0 && x > xS))
		return false;

	return CGameObject::IsOverlapping(obj);;
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

