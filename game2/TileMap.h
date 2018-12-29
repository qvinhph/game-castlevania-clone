#pragma once
#include <nlohmann/json.hpp>
#include "Tileset.h"
#include "GameObject.h"

using json = nlohmann::json;


#pragma region Some struct help containing infomation

struct CLayerInfo
{
	std::string name;
	int columns;
	int rows;
	vector<int> data;

	CLayerInfo(string name, int columns, int rows, vector<int> data)
		: name(name), columns(columns), rows(rows), data(data) {}
};
typedef CLayerInfo * LPLAYERINFO;


struct CObjectInfo
{
	std::string name;
	float x;
	float y;
	int nx;				// Direction of the object
	float width;
	float height;
	Item itemholding;
	string type;		// Multiply use with many different object

	CObjectInfo(string name, float x, float y, int nx, float width, float height, string type, Item item)
		: name(name), x(x), y(y), nx(nx), width(width), height(height), itemholding(item), type(type) {}
};
typedef CObjectInfo * LPOBJECTINFO;

#pragma endregion


class CTileMap
{
	int columns;							// map width by tiles
	int rows;								// map height by tiles

	CTileset* tileset;						// pointer to the matched tileset for manipulation
	LPCWSTR jsonFilePath;					// dir to json file contains the tileset information

	vector<LPLAYERINFO> layers;				// a vector of layers we should load to map
	vector<LPGAMEOBJECT> gameObjects;		// a vector of game objects in the tile map

public:

	CTileMap(LPCWSTR jsonFilePath) : jsonFilePath(jsonFilePath) {}

	void Init(int tilesetTextureID);
	void Draw();
	void Draw(float &left, float &top, float &right, float &bottom);
	void GetMapSize(int &width, int &height);
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void GetGameObjects(vector<LPGAMEOBJECT> * &objects) { objects = &this->gameObjects; }

	void CreateGameObjects(vector<LPOBJECTINFO> * objectsInfo);		// create game objects from the given info
	vector<LPLAYERINFO> GetTileLayers(json root);					// return a vector of layers in the json tilemap if has
	vector<LPOBJECTINFO> GetObjects(json root);						// return a vector of objects in the json tilemap if has
	Item GetHoldingItem(string string);								// return DropableItem value by string given.

};

