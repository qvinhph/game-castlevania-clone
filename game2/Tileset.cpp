#include <fstream>
#include <nlohmann/json.hpp>

#include "Tileset.h"
#include "debug.h"
#include "Sprites.h"
#include "Textures.h"

using json = nlohmann::json;

CTileset::CTileset(int textureID, LPCWSTR jsonFilePath, int firstGrid)
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
	
	if (root["margin"] != 0 && root["spacing"] != 0)
	{
		DebugOut(L"\nTileset image was not in a correct format\n");
		return;
	}
	else 
	{
		tileHeight = root["tileheight"].get<int>();
		tileWidth = root["tilewidth"].get<int>();
		rows = root["imageheight"].get<int>() / tileHeight;
		colums = root["imagewidth"].get<int>() / tileWidth;


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
