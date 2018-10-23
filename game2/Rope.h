#pragma once
#include "MovableObject.h"

#define FRONT_ROPE_LV1_BBOX_HEIGHT		18
#define FRONT_ROPE_LV1_BBOX_WIDTH		56
#define FRONT_ROPE_LV2_BBOX_HEIGHT		12
#define FRONT_ROPE_LV2_BBOX_WIDTH		56
#define FRONT_ROPE_LV3_BBOX_HEIGHT		12
#define FRONT_ROPE_LV3_BBOX_WIDTH		89

#define BACK_ROPE_BBOX_WIDTH			32

#define ROPE_STATE_LEVEL1				10
#define ROPE_STATE_LEVEL2				11
#define ROPE_STATE_LEVEL3				12

// for adjusting the rope to fit simon hand
#define DISTANCE_BACK_ROPE_AND_SIMON_POS		4
#define DISTANCE_FRONT_ROPE_AND_SIMON_POS		12

enum RopeAniID
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

	bool visible;
	
public:
	void LevelUp();

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;

	void RenderAnimation(int aniID) override;
	void SetVisible(bool isUsed);

	static CRope * GetInstance();
};

