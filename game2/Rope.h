#pragma once
#include "MovableObject.h"

#define FRONT_ROPE_LV1_BBOX_HEIGHT	18.0f
#define FRONT_ROPE_LV1_BBOX_WIDTH	56.0f
#define FRONT_ROPE_LV2_BBOX_HEIGHT	18.0f
#define FRONT_ROPE_LV2_BBOX_WIDTH	56.0f
#define FRONT_ROPE_LV3_BBOX_HEIGHT	18.0f
#define FRONT_ROPE_LV3_BBOX_WIDTH	89.0f

#define BACK_ROPE_BBOX_WIDTH		32
#define BACK_ROPE_BBOX_HEIGHT		38

#define ROPE_LEVEL1					10
#define ROPE_LEVEL2					11
#define ROPE_LEVEL3					12

#define ROPE_DAMAGE_LEVEL1			1
#define ROPE_DAMAGE_LEVEL2			2
	
// for adjusting the rope to fit simon hand
#define Y_DISTANCE_FRONTROPE_LOWER_SIMON		14

enum class RopeAniID
{
	LEVEL_ONE_RIGHT = 9800,
	LEVEL_ONE_LEFT,
	LEVEL_TWO_RIGHT,
	LEVEL_TWO_LEFT,
	LEVEL_THREE_RIGHT,
	LEVEL_THREE_LEFT,
};

class CRope: public CActiveObject
{
	static CRope * __instance;
	CRope();
	int level;
	
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;

	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;
	void Render() override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void SetState(int state) override;
	bool IsOverlapping(LPGAMEOBJECT obj) override;
	bool IsInViewport() override;

	void UpdateRopePosition(int aniID);
	void SetMatchedAnimation();
	void LevelUp();
	void PickDamage();

	static CRope * GetInstance();
};

