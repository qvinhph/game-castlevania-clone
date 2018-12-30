#pragma once
#include "Portal.h"
#include "GameObject.h"

/*
	A portals manager class help Simon performs the teleport.
*/
class CPortals
{
	static CPortals * __instance;

	vector<CPortal *> portals;
	CPortals() = default;

public:

	void Add(LPGAMEOBJECT teleport);
	void Add(CPortal * teleport);

	/*
		Return the position of the next port depending on its indicator
	*/
	void Teleport(CPortal * portal, float &arrivalX, float &arrivalY);
	static CPortals * GetInstance();

};

