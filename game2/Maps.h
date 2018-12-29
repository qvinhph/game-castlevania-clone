#pragma once
#include "Map.h"
#include <unordered_map>

using namespace std;

class CMaps
{

	static CMaps * __instance;
	CMaps() = default;

	vector<LPMAP> maps;
	int currentMapIndex = 0;

public:

	void SetCurrentMap(int currentMapIndex) { this->currentMapIndex = currentMapIndex; }
	LPMAP GetCurrentMap() { return this->maps[currentMapIndex]; }
	void AddMap(LPMAP map);

	// TO-DO: Need something to help player reach the next map by changing the currentMapIndex

	static CMaps * GetInstance();
};

