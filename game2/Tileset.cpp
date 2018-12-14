#include <fstream>
#include <nlohmann/json.hpp>

#include "Tileset.h"
#include "debug.h"
#include "Sprites.h"
#include "Textures.h"

#define TILESET_KEY_MARGIN				"margin"
#define TILESET_KEY_SPACING				"spacing"
#define TILESET_KEY_TILEHEIGHT			"tileheight"
#define TILESET_KEY_TILEWIDTH			"tilewidth"
#define TILESET_KEY_IMAGEHEIGHT			"imageheight"
#define TILESET_KEY_IMAGEWIDTH			"imagewidth"

using json = nlohmann::json;

CTileset::CTileset(int textureID, string jsonFilePath, int firstGrid)
	: jsonFilePath(jsonFilePath), firstGrid(firstGrid)
{
	this->texture = CTextures::GetInstance()->Get(textureID);
}

void CTileset::Init()
{
	ifstream inputFile(jsonFilePath);
	json root;
	inputFile >> root;
	inputFile.close();
	
	if (root[TILESET_KEY_MARGIN] != 0 && root[TILESET_KEY_SPACING] != 0)
	{
		DebugOut(L"\n[ERROR] Tileset image was not in a correct format\n");
		return;
	}
	else 
	{
		tileHeight = root[TILESET_KEY_TILEHEIGHT].get<int>();
		tileWidth = root[TILESET_KEY_TILEWIDTH].get<int>();
		rows = root[TILESET_KEY_IMAGEHEIGHT].get<int>() / tileHeight;
		columns = root[TILESET_KEY_IMAGEWIDTH].get<int>() / tileWidth;

		LPTILE tile;
		int gridID = firstGrid;
		tiles.resize(rows * columns + firstGrid);

		for (UINT row = 0; row < rows; row++)
			for (UINT col = 0; col < columns; col++)
			{
				tile = new CTile(col * tileWidth, row * tileHeight,
								(col + 1) * tileWidth, (row + 1) * tileHeight, texture);

				tiles[gridID] = tile;
				gridID++;
			}
	}
}

void CTileset::Get(int grid, LPTILE &tile)
{
	tile = tiles[grid];
}
