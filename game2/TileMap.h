#pragma once
#include <nlohmann/json.hpp>
#include "Tileset.h"

using json = nlohmann::json;

struct CLayerInfo
{
	std::string name;
	int width;
	int height;
	vector<int> data;

	CLayerInfo(string name, int width, int height, vector<int> data)
	{
		this->name = name;
		this->width = width;
		this->height = height;
		this->data = data;
	}
};
typedef CLayerInfo * LPLAYERINFO;

class CTileMap
{

	int width;									// map size by tiles
	int height;

	CTileset* tileset;
	LPCWSTR jsonFilePath;							// dir to json file contains the tileset information

	vector<LPLAYERINFO> layers;						// a collection of layers we should load to map
	vector<LPLAYERINFO> GetTileLayers(json root);	// return vector of layers in json object if has

public:

	CTileMap(LPCWSTR jsonFilePath) : jsonFilePath(jsonFilePath) {}

	void Init(int tilesetTextureID);
	void Draw();

};

