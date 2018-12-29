#pragma once
#include "TileMap.h"
#include "Cells.h"

class CMap
{

	CTileMap * tileMap;
	CCells * cells;

public:

	CMap(LPCWSTR jsonFilePath);
	CCells * GetCells() { return this->cells; }
	CTileMap * GetTileMap() { return this->tileMap; }
};

typedef CMap * LPMAP;

