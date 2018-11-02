#pragma once
#include <unordered_map>
#include <d3dx9.h>

#include "Tile.h"

using namespace std;

/*
	Class containing the tiles using vector<LPTILE>
*/
class CTileset
{
	unordered_map<int, LPTILE> tiles;
	int tileHeight;					// height of a single tile
	int tileWidth;					// width of a single tile

	int colums;
	int rows;
	int firstGrid;

	LPDIRECT3DTEXTURE9 texture;			// the image source used for tileset
	LPCWSTR jsonFilePath;				// dir to json file contains the tileset information
	
public:

	CTileset(LPDIRECT3DTEXTURE9 tex, LPCWSTR jsonFilePath, int firstGrid)
		: texture(tex), jsonFilePath(jsonFilePath), firstGrid(firstGrid) {}

	CTileset(int textureID, LPCWSTR jsonFilePath, int firstGrid);

	void Init();
	LPTILE Get(int grid);
	int GetTileHeight() { return this->tileHeight; }
	int GetTileWidth() { return this->tileWidth; }

};
