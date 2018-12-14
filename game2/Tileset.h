#pragma once
#include <unordered_map>
#include <d3dx9.h>

#include "Tile.h"

#define TILESET_CONTAINER_DIR		"json\\"

using namespace std;

/*
	Class containing the tiles using vector<LPTILE>
*/
class CTileset
{
	//unordered_map<int, LPTILE> tiles;
	vector<LPTILE> tiles;
	int tileHeight;							// height of a single tile
	int tileWidth;							// width of a single tile

	int columns;
	int rows;
	int firstGrid;

	LPDIRECT3DTEXTURE9 texture;				// the image source used for tileset
	string jsonFilePath;					// dir to json file contains the tileset information
	
public:

	CTileset(LPDIRECT3DTEXTURE9 tex, string jsonFilePath, int firstGrid)
		: texture(tex), jsonFilePath(jsonFilePath), firstGrid(firstGrid) {}

	CTileset(int textureID, string jsonFilePath, int firstGrid);

	void Init();
	void CTileset::Get(int grid, LPTILE &tile);
	int GetFirstGrid()	{ return this->firstGrid; }
	int GetTileHeight() { return this->tileHeight; }
	int GetTileWidth()	{ return this->tileWidth; }

};

