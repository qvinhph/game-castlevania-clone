#pragma once
#include <nlohmann/json.hpp>
#include "Tileset.h"
#include "GameObject.h"

using json = nlohmann::json;


#pragma region Some struct help containing infomation

struct CLayerInfo
{
	std::string name;
	int width;
	int height;
	vector<int> data;

	CLayerInfo(string name, int width, int height, vector<int> data)
		: name(name), width(width), height(height), data(data) {}
};
typedef CLayerInfo * LPLAYERINFO;


struct CObjectInfo
{
	std::string name;
	float x;
	float y;
	int nx;
	float width;
	float height;
	Item dropableItem;

	CObjectInfo(string name, float x, float y, int nx, float width, float height, Item item)
		: name(name), x(x), y(y), nx(nx), width(width), height(height), dropableItem(item) {}
};
typedef CObjectInfo * LPOBJECTINFO;

#pragma endregion


class CTileMap
{

	int width;												// map width by tiles
	int height;												// map height by tiles

	CTileset* tileset;
	LPCWSTR jsonFilePath;									// dir to json file contains the tileset information

	vector<LPLAYERINFO> layers;								// a vector of layers we should load to map
	vector<LPOBJECTINFO> objects;							// a vector of game objects

public:

	CTileMap(LPCWSTR jsonFilePath) : jsonFilePath(jsonFilePath) {}

	void Init(int tilesetTextureID);
	void Draw();

	vector<LPGAMEOBJECT> GetGameObjects();					// return a vector of game objects in the map
	static vector<LPLAYERINFO> GetTileLayers(json root);	// return a vector of layers in the json tilemap if has
	static vector<LPOBJECTINFO> GetObjects(json root);		// return a vector of objects in the json tilemap if has
	static Item GetHoldingItem(string string);				// return DropableItem value by string given.

};

