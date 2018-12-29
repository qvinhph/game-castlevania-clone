#include "Map.h"
#include "Castlevania.h"

CMap::CMap(LPCWSTR jsonFilePath)
{
	this->tileMap = new CTileMap(jsonFilePath);
	this->tileMap->Init(ID_TEX_TILESET);
	this->cells = new CCells();
	this->cells->Init(tileMap, CELL_WIDTH, CELL_HEIGHT);
}
