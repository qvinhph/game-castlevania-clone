#pragma once
#include "GameObject.h"

#define GATE_BBOX_WIDTH			16
#define GATE_BBOX_HEIGHT		96

#define GATE_OPEN_TIME			2000

enum class GateAniID
{
	CLOSING = 2400,
	OPENING
};

class CGate : public CGameObject
{
	bool openning = false;

	// Timers, also be used as FLAG for the action when assigned to TIMER_IDLE(false) or non-TIMER_IDLE(true)
	// Assigned to TIMER_IDLE			: the timer is not working,
	// Assigned to TIMER_ETERNAL		: the timer is working and the action is not going to stop
	// Assigned to unsigned number		: the timer is working
	DWORD attack_start;
	DWORD open_start = TIMER_IDLE;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override;

	bool GetOpenning() { return this->openning; }
	void SetOpenning(bool openning) { this->openning = openning; this->open_start = GetTickCount(); }
};

