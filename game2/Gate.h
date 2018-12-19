#pragma once
#include "GameObject.h"

#define GATE_BBOX_WIDTH			16
#define GATE_BBOX_HEIGHT		96

enum class GateAniID
{
	CLOSING = 2400,
	OPENING
};

class CGate : public CGameObject
{
	bool closing = true;
	

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override;

	bool GetClosing() { return this->closing; }
	void SetClosing(bool closing) { closing =  this->closing; }
};

