#pragma once
#include "MovableObject.h"

#define BAT_BBOX_WIDTH				32
#define BAT_BBOX_HEIGHT				32

#define BAT_SPEED_VX				0.14
#define BAT_SPEED_VY				0.06
#define BAT_MOVEMENT_AMPLITUDE		20

#define PINK_BAT_DEFAULT_HEALTH		1
#define PINK_BAT_DAMAGE				2
#define PINK_BAT_POINT_AWARD		200

enum class PinkBatAniID
{
	FLYING_LEFT = 5301,
	FLYING_RIGHT = 5302,
};

/*
	Pink Bat is a monster without idle state.
*/
class CPinkBat: public CActiveObject
{
	float line;		// the horizontal axis the bat will move around

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) override;

	void SetLine(float line) { this->line = line; }
	float GetLine() { return this->line; }

	void PickAnimation();
	CPinkBat();

};

