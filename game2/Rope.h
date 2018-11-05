#pragma once
#include "MovableObject.h"

#define FRONT_ROPE_LV1_BBOX_HEIGHT		18.0f
#define FRONT_ROPE_LV1_BBOX_WIDTH		56.0f
#define FRONT_ROPE_LV2_BBOX_HEIGHT		18.0f
#define FRONT_ROPE_LV2_BBOX_WIDTH		56.0f
#define FRONT_ROPE_LV3_BBOX_HEIGHT		18.0f
#define FRONT_ROPE_LV3_BBOX_WIDTH		89.0f

#define BACK_ROPE_BBOX_WIDTH			32

#define ROPE_LEVEL1						10
#define ROPE_LEVEL2						11
#define ROPE_LEVEL3						12
	
// for adjusting the rope to fit simon hand
#define Y_DISTANCE_BACK_ROPE_AND_SIMON_POS		4
#define Y_DISTANCE_FRONT_ROPE_AND_SIMON_POS		12

enum class RopeAniID
{
	LEVEL_ONE_RIGHT = 9800,
	LEVEL_ONE_LEFT,
	LEVEL_TWO_RIGHT,
	LEVEL_TWO_LEFT,
	LEVEL_THREE_RIGHT,
	LEVEL_THREE_LEFT
};

class CRope: public CMovableObject
{
	static CRope * __instance;
	CRope();
	int level;
	
public:

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;

	// TO-DO: Maybe need to refactoring
	void ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents) override;
	void Render() override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void SetState(int state) override;

	void UpdateRopePosition(int aniID);
	void SetMatchedAnimation();
	void LevelUp();
	bool IsOverlapping(LPGAMEOBJECT obj);

	static CRope * GetInstance();
};

