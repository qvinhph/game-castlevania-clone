#include <fstream>
#include <nlohmann/json.hpp>

#include "TileMap.h"
#include "debug.h"
#include "JsonUtility.h"


void CTileMap::Init(int tilesetTextureID)
{
	ifstream inputFile(jsonFilePath);
	json root;
	inputFile >> root;
	inputFile.close();

	if (root["type"] != "map")
	{
		DebugOut(L"\Json Tile Map was not in a correct format.");
		return;
	}

	if (root["tilesets"].size() > 1)	// value mapped by "tilesets" is a json array
	{
		DebugOut(L"\nThis TileMap class haven't supported loading from multi-tilesets now.");
		return;
	}
	
	height = root["height"].get<int>();
	width = root["width"].get<int>() ;
	LPCWSTR tilesetPath = L"json\\tileset-info.json";
	int firstGrid;
	
	json tilesetInfo = *root["tilesets"].begin();
	firstGrid = tilesetInfo["firstgid"].get<int>();
	//string path = tilesetInfo["source"].get<string>();
	//tilesetPath = L"%s", path;				// convert string to LPCWSTR

	tileset = new CTileset(tilesetTextureID, tilesetPath, firstGrid);
	tileset->Init();
	layers = GetTileLayers(root);
	
}

void CTileMap::Draw()
{
	for (UINT i = 0; i < layers.size(); i++)
	{
		int index = 0;		// the index in data array

		for (UINT row = 0; row < layers[i]->height; row++)
			for (UINT column = 0; column < layers[i]->width; column++)
			{
				int gridID = layers[i]->data.at(index++);
				if (gridID == 0) continue;			// empty tile

				tileset->Get(gridID)->Draw(column * tileset->GetTileWidth(),
										row * tileset->GetTileHeight());
			}

	}
}

vector<LPLAYERINFO> CTileMap::GetTileLayers(json root)
{
	vector<LPLAYERINFO> result;

	json layerArray = JSONUtility::GetObjectArray(root, "layers");

	if (layerArray.size() == 0)
	{
		DebugOut(L"Json Tile Map was not in a correct format");
		return vector<LPLAYERINFO>();					// return an empty vector.
	}

	// because array value mapped with "layers" may contains object information
	// we need loop through the array to get layers
	for (auto it = layerArray.begin(); it != layerArray.end(); ++it)
	{
		json jsonObj = *it;

		if (jsonObj["type"] == "tilelayer")		
		{
			vector<int> data;
			json dataArray = JSONUtility::GetObjectArray(jsonObj, "data");
			for (auto it = dataArray.begin(); it != dataArray.end(); ++it)
				if ((*it).is_number_integer())
					data.push_back(*it);
				else
				{
					DebugOut(L"Layer data was not in a correct format");
					return vector<LPLAYERINFO>();			// return an empty vector.
				}

			result.push_back(
				new CLayerInfo(jsonObj["name"].get<string>(),
					jsonObj["width"].get<int>(),
					jsonObj["height"].get<int>(),
					dataArray));
		}
	}

	return result;
}

