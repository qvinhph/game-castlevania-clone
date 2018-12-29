#include "Maps.h"

CMaps * CMaps::__instance = NULL;

void CMaps::AddMap(LPMAP map)
{
	maps.push_back(map);
}

CMaps * CMaps::GetInstance()
{
	if (__instance == NULL)
		__instance = new CMaps();

	return __instance;
}
