#pragma once
#include "GameObject.h"

enum class PortalIndicator
{
	NEXT,
	BACK,
	NONE
};

class CPortal : public CGameObject
{
	float width;
	float height;
	int idPort;

	PortalIndicator indicator = PortalIndicator::NONE;

public:

	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override { /* Render nothing because this is an invisible object */ }

	void SetIDPort(int index) { this->idPort = index; }
	int GetIDPort() { return this->idPort; }

	void SetIndicator(PortalIndicator indicator) { this->indicator = indicator; }
	PortalIndicator GetIndicator() { return this->indicator; }

	CPortal(float width, float height, PortalIndicator indicator) 
		: width(width), height(height), indicator(indicator) {}
};

