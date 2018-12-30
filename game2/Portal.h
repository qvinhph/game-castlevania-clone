#pragma once
#include "GameObject.h"

enum class PortalIndicator
{
	NEXT,
	BACK,
	NONE
};


/*
	When Simon touches an instance of this class, he might perform a teleport to another Portal.
	( depend on the PortalIndicator variable ).

	NOTE: All of the instances of this class will be managed by CPortals
*/
class CPortal : public CGameObject
{
	float width;
	float height;

	PortalIndicator indicator = PortalIndicator::NONE;

	/*
		To mark the order of portals
		TO-DO: Need more thought about this. Maybe this is unnecessary.
	*/
	int idPort;

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

