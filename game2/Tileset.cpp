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
		colums = root[TILESET_KEY_IMAGEWIDTH].get<int>() / tileWidth;


		LPTILE tile;
		int gridID = firstGrid;
		for (UINT i = 0; i < rows; i++)
			for (UINT j = 0; j < colums; j++)
			{
				tile = new CTile(j * tileWidth, i * tileHeight,
					(j + 1) * tileWidth, (i + 1) * tileHeight, texture);

				tiles[gridID] = tile;
				gridID++;
			}
	}
}

LPTILE CTileset::Get(int grid)
{
	return tiles[grid];
}
