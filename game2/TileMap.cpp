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
		height = root[TILEMAP_KEY_HEIGHT].get<int>();
		width = root[TILEMAP_KEY_WIDTH].get<int>();

		string tilesetPath;
		int firstGrid;

		json tilesetInfo = *root[TILEMAP_KEY_TILESETS].begin();	
		firstGrid = tilesetInfo[TILEMAP_KEY_FIRSTGRID].get<int>();
		tilesetPath = TILESET_DIR + tilesetInfo[TILEMAP_KEY_SOURCE].get<string>();

		tileset = new CTileset(tilesetTextureID, tilesetPath, firstGrid);
		tileset->Init();
		layers = GetTileLayers(root);
		objects = GetObjects(root);
	}
}

void CTileMap::Draw()
{
	for (UINT i = 0; i < layers.size(); i++)
	{
		int index = 0;		// the index in data array

		for (UINT row = 0; row < layers[i]->height; row++)
			for (UINT column = 0; column < layers[i]->width; column++)
			{
				int gridID = layers[i]->data[index];
				index++;

				if (gridID < tileset->GetFirstGrid()) continue;			// empty tile
				tileset->Get(gridID)->Draw(column * tileset->GetTileWidth(),
											row * tileset->GetTileHeight());
			}

	}
}

vector<LPGAMEOBJECT> CTileMap::GetGameObjects()
{
	vector<LPGAMEOBJECT> result;
	LPOBJECTINFO info;
	LPGAMEOBJECT obj;
	CItems * items = CItems::GetInstance();			// For adding and managing the item-type objects
	CFlames * flames = CFlames::GetInstance();		// For adding and managing the destroying flames
	CWeapons * weapons = CWeapons::GetInstance();	// For adding and managing weapon-type objects
	

	for (UINT i = 0; i < objects.size(); i++)
	{
		info = objects[i];

		if		(info->name == "bigcandle")			obj = new CBigCandle();
		else if (info->name == "candle")			obj = new CCandle();
		else if (info->name == "rope")				obj = CRope::GetInstance();
		else if (info->name == "simon")				obj = CSimon::GetInstance();
		else if (info->name == "stairs_up")		
		{
			obj = new CStairsUp();
			obj->SetDirection(info->nx);
		}

		else if (info->name == "stairs_down")
		{
			obj = new CStairsDown();
			obj->SetDirection(info->nx);
		}


		// Monsters
		else if (info->name == "zombie")			obj = new CZombie();
		else if (info->name == "panther")			obj = new CPanther();


		// Item-type and dropable game objects
		else if (info->name == "itemrope")
		{
			obj = new CItemRope();
			items->Add(Item::ITEMROPE, obj);
		}
		else if (info->name == "bigheart")
		{
			obj = new CBigHeart();
			items->Add(Item::BIGHEART, obj);
		}
		else if (info->name == "heart")
		{
			obj = new CHeart();
			items->Add(Item::HEART, obj);
		}
		else if (info->name == "itemdagger")
		{
			obj = new CItemDagger();
			items->Add(Item::ITEMDAGGER, obj);
		}


		// Weapon-type game objects
		else if (info->name == "dagger")
		{
			obj = new CDagger();
			weapons->Add(Weapon::DAGGER, obj);
		}


		// Destroying flame 
		else if (info->name == "flame")
		{
			obj = new CFlame();
			flames->Add((CFlame *)obj);
		}


		// Invisiblewall
		else if (info->name == "invisiblewall")
		{
			obj = new CInvisibleWall();
			dynamic_cast<CInvisibleWall *>(obj)->SetSize(info->width, info->height);
		}

		else
		{
			// In case something mismatched
			DebugOut(L"\n[ERROR] Load Game Objects failed. \n Cannot recognize something !!");
			return vector<LPGAMEOBJECT>();		// return empty vector
		}

		float x = info->x;
		float y = info->y;
		obj->SetPosition(x, y);
		obj->SetHoldingItem(info->dropableItem);
		result.push_back(obj);
	}

	return result;
}

vector<LPLAYERINFO> CTileMap::GetTileLayers(json root)
{
	vector<LPLAYERINFO> result;

	json layersArray = JSONUtility::GetObjectArray(root, TILEMAP_KEY_LAYERS);
	if (layersArray.size() == 0)
		DebugOut(L"\n[ERROR] Load map layers failed");
	
	else
	{
		json jsonObj;
		for (json::iterator it = layersArray.begin(); it != layersArray.end(); ++it)
		{
			jsonObj = *it;

			if (jsonObj[TILEMAP_KEY_TYPE] == "tilelayer")
			{
				vector<int> data;
				json dataArray = JSONUtility::GetObjectArray(jsonObj, TILEMAP_KEY_DATA);
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
						jsonObj[TILEMAP_KEY_NAME].get<string>(),
						jsonObj[TILEMAP_KEY_WIDTH].get<int>(),
						jsonObj[TILEMAP_KEY_HEIGHT].get<int>(),
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


