#include "Portals.h"

CPortals * CPortals::__instance = NULL;


void CPortals::Add(LPGAMEOBJECT teleport)
{
	CPortal * portal = dynamic_cast<CPortal *>(teleport);
	this->Add(portal);	
}

void CPortals::Add(CPortal * teleport)
{
	teleport->SetIDPort(portals.size());
	portals.push_back(teleport);
}

void CPortals::Teleport(CPortal * portal, float & arrivalX, float & arrivalY)
{
	PortalIndicator indicator = portal->GetIndicator();
	int index = portal->GetIDPort();


	// Choose arrival portal
	CPortal * arrivalPortal = NULL;
	switch (indicator)
	{
	case PortalIndicator::NEXT:
		// Not the last in the list
		if (index != portals.size() - 1)
			arrivalPortal = portals[index + 1];
		break;

	case PortalIndicator::BACK:
		// Not the first in the list
		if (index != 0)
			arrivalPortal = portals[index - 1];
		break;

	default:
		arrivalPortal = NULL;
		break;
	}


	// Get position
	if (arrivalPortal != NULL)
	{
		arrivalPortal->GetPosition(arrivalX, arrivalY);

		// Get the camera focus for player
		CCamera::GetInstance()->ChangeLimitBound(arrivalX, arrivalY);
	}
}

CPortals * CPortals::GetInstance()
{
	if (__instance == NULL)
		__instance = new CPortals();

	return __instance;
}

