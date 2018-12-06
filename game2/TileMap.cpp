#include <fstream>
#include <nlohmann/json.hpp>

#include "TileMap.h"
#include "debug.h"
#include "JsonUtility.h"
#include "Castlevania.h"

#define TILEMAP_KEY_TYPE					"type"
#define TILEMAP_KEY_SOURCE					"source"
#define TILEMAP_KEY_TILESETS				"tilesets"
#define TILEMAP_KEY_FIRSTGRID				"firstgid"
#define TILEMAP_KEY_HEIGHT					"height"
#define TILEMAP_KEY_WIDTH					"width"
#define TILEMAP_KEY_LAYERS					"layers"
#define TILEMAP_KEY_DATA					"data"
#define TILEMAP_KEY_NAME					"name"
#define TILEMAP_KEY_VALUE					"value"
#define TILEMAP_KEY_HEIGHT					"height"
#define TILEMAP_KEY_WIDTH					"width"
#define TILEMAP_KEY_OBJECTS					"objects"
#define TILEMAP_KEY_X_POSITION				"x"
#define TILEMAP_KEY_Y_POSITION				"y"

void CTileMap::Init(int tilesetTextureID)
{
	ifstream inputFile(jsonFilePath);
	json root;
	inputFile >> root;
	inputFile.close();

	if (root[TILEMAP_KEY_TYPE] != "map")
		DebugOut(L"\n[ERROR] Json Tile Map not found.");

	else if (root[TILEMAP_KEY_TILESETS].size() > 1)	// value mapped by "tilesets" is a json array
		DebugOut(L"\n[ERROR] This TileMap class haven't supported loading from multi-tilesets yet.");

	else
	{
		this->rows = root[TILEMAP_KEY_HEIGHT].get<int>();
		this->columns = root[TILEMAP_KEY_WIDTH].get<int>();

		string tilesetPath;
		int firstGrid;

		json tilesetInfo = *root[TILEMAP_KEY_TILESETS].begin();	
		firstGrid = tilesetInfo[TILEMAP_KEY_FIRSTGRID].get<int>();
		tilesetPath = TILESET_CONTAINER_DIR + tilesetInfo[TILEMAP_KEY_SOURCE].get<string>();

		this->tileset = new CTileset(tilesetTextureID, tilesetPath, firstGrid);
		this->tileset->Init();

		// Get the layers
		this->layers = GetTileLayers(root);

		// Create the game objects from the objects' information.
		vector<LPOBJECTINFO> objectsInfo = GetObjects(root);
		this->CreateGameObjects(&objectsInfo);
	}
}

void CTileMap::Draw()
{
	for (UINT i = 0; i < layers.size(); i++)
	{
		int index = 0;		// the index in data array
		int gridID;

		for (UINT row = 0; row < layers[i]->rows; row++)
			for (UINT column = 0; column < layers[i]->columns; column++)
			{
				gridID = layers[i]->data[index];
				index++;

				if (gridID < tileset->GetFirstGrid()) continue;			// empty tile

				tileset->Get(gridID)->Draw(column * tileset->GetTileWidth(),		// TO-DO: optmize by use variable
											row * tileset->GetTileHeight());		// tileWidth and tileHeight;
			}

	}
}

void CTileMap::Draw(float & left, float & top, float & right, float & bottom)
{
	// Find the top-left and bottom-right tile that contains the viewport's area
	int tileHeight = tileset->GetTileHeight();
	int tileWidth = tileset->GetTileWidth();

	int firstColumn, firstRow;		// The top-right tile that has the viewport's area
	int lastColumn, lastRow;		// The bottom-right tile that has the viewport's area

	firstColumn = left / tileWidth;
	lastColumn = (right / tileWidth == columns) ? columns - 1 : (right / tileWidth);

	firstRow = top / tileHeight;
	lastRow = (bottom / tileHeight == rows) ? rows - 1 : (bottom / tileHeight);

	// Draw the tiles bounded by top-left and bottom-right tile
	int index;
	int gridID;
	for (UINT row = firstRow; row <= lastRow; ++row)
		for (UINT column = firstColumn; column <= lastColumn; ++column)
		{
			index = row * columns + column;

			// Get grid ID
			gridID = layers[0]->data[index];
			if (gridID < tileset->GetFirstGrid()) continue;		// empty tile

			tileset->Get(gridID)->Draw(column * tileWidth, row * tileHeight);
		}
}

void CTileMap::GetMapSize(int & width, int & height)
{
	width = this->columns * tileset->GetTileWidth();
	height = this->rows * tileset->GetTileHeight();
}

void CTileMap::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	int mapWidth, mapHeight;
	this->GetMapSize(mapWidth, mapHeight);

	left = 0;
	top = 0;
	right = left + mapWidth;
	bottom = top + mapHeight;
}

void CTileMap::CreateGameObjects(vector<LPOBJECTINFO> * objectsInfo)
{

	LPOBJECTINFO info;
	LPGAMEOBJECT obj;


	for (UINT i = 0; i < objectsInfo->size(); i++)
	{
		info = objectsInfo->at(i);

		// Static objects
		if (info->name == "bigcandle")				obj = new CBigCandle();
		else if (info->name == "candle")			obj = new CCandle();
		else if (info->name == "stairs_up")			obj = new CStairsUp();
		else if (info->name == "stairs_down")		obj = new CStairsDown();


		// Monsters
		else if (info->name == "zombie")			obj = new CZombie();
		else if (info->name == "panther")			obj = new CPanther();
		else if (info->name == "pinkbat")			obj = new CPinkBat();

		// Monsters' spawner
		else if (info->name == "spawnerzombie")		obj = new CSpawnerZombie(info->width, info->height);
		else if (info->name == "spawnerpinkbat")	obj = new CSpawnerPinkBat(info->width, info->height);
		else if (info->name == "spawnerpanther")	obj = new CSpawnerPanther();

		// Invisiblewall
		else if (info->name == "invisiblewall")		obj = new CInvisibleWall(info->width, info->height);


		// Simon
		else if (info->name == "simon")
		{
			obj = CSimon::GetInstance();
			obj->SetPosition(info->x, info->y);
			continue;
		}


		else
		{
			// In case something mismatched
			DebugOut(L"\n[ERROR] Load Game Objects failed. \n Cannot recognize something !!");
			this->gameObjects.clear();
			break;
		}


		obj->SetPosition(info->x, info->y);
		obj->SetHoldingItem(info->dropableItem);

		if (info->nx != 0)
			obj->SetDirection(info->nx);

		this->gameObjects.push_back(obj);
	}
}

vector<LPLAYERINFO> CTileMap::GetTileLayers(json root)
{
	vector<LPLAYERINFO> result;

	json layersArray = JSONUtility::GetObjectArray(root, TILEMAP_KEY_LAYERS);
	if (layersArray.size() == 0)
		DebugOut(L"\n[ERROR] Load map layers failed");
	
	else
	{
		json layer;
		for (json::iterator it = layersArray.begin(); it != layersArray.end(); ++it)
		{
			layer = *it;

			if (layer[TILEMAP_KEY_TYPE] == "tilelayer")
			{
				vector<int> data;
				json dataArray = JSONUtility::GetObjectArray(layer, TILEMAP_KEY_DATA);
				for (auto it = dataArray.begin(); it != dataArray.end(); ++it)
					if ((*it).is_number_integer())
						data.push_back(*it);
					else
					{
						DebugOut(L"\n[ERROR] Layer data was not in a correct format");
						return vector<LPLAYERINFO>();			// return an empty vector.
					}

				result.push_back(
					new CLayerInfo(
						layer[TILEMAP_KEY_NAME].get<string>(),
						layer[TILEMAP_KEY_WIDTH].get<int>(),
						layer[TILEMAP_KEY_HEIGHT].get<int>(),
						dataArray));
			}
		}
	}
	
	return result;
}

vector<LPOBJECTINFO> CTileMap::GetObjects(json root)
{
	vector<LPOBJECTINFO> result;

	json layers = JSONUtility::GetObjectArray(root, TILEMAP_KEY_LAYERS);
	if (layers.size() == 0)
		DebugOut(L"\n[ERROR] Load map layers failed");
	
	else 
	{
		for (json::iterator layer = layers.begin(); layer != layers.end(); ++layer)
		{
			if ((*layer)[TILEMAP_KEY_TYPE] == "objectgroup")
			{
				json objects = JSONUtility::GetObjectArray(*layer, TILEMAP_KEY_OBJECTS);
				
				for (json::iterator obj = objects.begin(); obj != objects.end(); ++obj)
				{

					Item item = Item::NONE;
					int nx = 0;

					// To get dropable item of the object, if has 
					json properties = JSONUtility::GetObjectArray(*obj, "properties");
					if (properties.is_array())
						for (json::iterator prop = properties.begin(); prop != properties.end(); ++prop)
						{
							if ((*prop)[TILEMAP_KEY_NAME] == "itemholding")
								item = ((*prop)[TILEMAP_KEY_VALUE].is_null()) ?
								Item::NONE :
								GetHoldingItem((*prop)[TILEMAP_KEY_VALUE]);

							else if ((*prop)[TILEMAP_KEY_NAME] == "Direction")
								nx = (*prop)[TILEMAP_KEY_VALUE].get<int>();

							else
								DebugOut(L"\n[ERROR] Can't read this property from JSON Tile Map");
						}
							

					result.push_back(
						new CObjectInfo(
						(*obj)[TILEMAP_KEY_NAME].get<string>(),
							(*obj)[TILEMAP_KEY_X_POSITION].get<float>(),
							(*obj)[TILEMAP_KEY_Y_POSITION].get<float>(),
							nx,
							(*obj)[TILEMAP_KEY_WIDTH].get<float>(),
							(*obj)[TILEMAP_KEY_HEIGHT].get<float>(),
							item));
				}
			}
		}
	}	

	return result;
}

Item CTileMap::GetHoldingItem(string string)
{
	if (string == "itemrope")		return Item::ITEMROPE;
	if (string == "bigheart")		return Item::BIGHEART;
	if (string == "heart")			return Item::HEART;
	if (string == "itemdagger")		return Item::ITEMDAGGER;

	return Item::NONE;
}


