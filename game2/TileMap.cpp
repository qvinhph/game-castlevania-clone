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
	{
		DebugOut(L"\n[ERROR] Json Tile Map was not in a correct format.");
		return;
	}

	if (root[TILEMAP_KEY_TILESETS].size() > 1)	// value mapped by "tilesets" is a json array
	{
		DebugOut(L"\n[ERROR] This TileMap class haven't supported loading from multi-tilesets yet.");
		return;
	}
	
	height = root[TILEMAP_KEY_HEIGHT].get<int>();
	width = root[TILEMAP_KEY_WIDTH].get<int>() ;

	string tilesetPath;
	int firstGrid;
	
	json tilesetInfo = *root[TILEMAP_KEY_TILESETS].begin();
	firstGrid = tilesetInfo[TILEMAP_KEY_FIRSTGRID].get<int>();
	tilesetPath = tilesetInfo[TILEMAP_KEY_SOURCE].get<string>();

	tileset = new CTileset(tilesetTextureID, tilesetPath, firstGrid);
	tileset->Init();
	layers = GetTileLayers(root);
	objects = GetObjects(root);
	
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

vector<LPGAMEOBJECT> CTileMap::GetGameObjects()
{
	vector<LPGAMEOBJECT> result;
	LPOBJECTINFO info;
	LPGAMEOBJECT obj;

	for (UINT i = 0; i < objects.size(); i++)
	{
		info = objects.at(i);

		if (info->name == "insiviblewall")
		{
			obj = new CInvisibleWall();
			dynamic_cast<CInvisibleWall *>(obj)->SetSize(info->width, info->height);
		}
		else if (info->name == "bigcandle") obj = new CBigCandle();
		else if (info->name == "itemrope")
		{
			obj = new CItemRope();
			CItems::GetInstance()->Add(Item::ITEMROPE, obj);
		}
		else if (info->name == "rope")		obj = CRope::GetInstance();
		else if (info->name == "simon")		obj = CSimon::GetInstance();
		else
		{
			DebugOut(L"[ERROR] Load Game Objects failed");
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
		DebugOut(L"Load map layers failed");
	
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
		DebugOut(L"Load map layers failed");
	
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

					// To get dropable item of the object, if has 
					json properties = JSONUtility::GetObjectArray(*obj, "properties");
					if (properties.is_array())
						for (json::iterator prop = properties.begin(); prop != properties.end(); ++prop)
							if ((*prop)[TILEMAP_KEY_NAME] == "itemholding")
								item = ((*prop)[TILEMAP_KEY_VALUE].is_null()) ?
								Item::NONE :
								GetDropableItem((*prop)[TILEMAP_KEY_VALUE]);

					result.push_back(
						new CObjectInfo(
						(*obj)[TILEMAP_KEY_NAME].get<string>(),
							(*obj)[TILEMAP_KEY_X_POSITION].get<float>(),
							(*obj)[TILEMAP_KEY_Y_POSITION].get<float>(),
							(*obj)[TILEMAP_KEY_WIDTH].get<float>(),
							(*obj)[TILEMAP_KEY_HEIGHT].get<float>(),
							item));
				}
			}
		}
	}	

	return result;
}

Item CTileMap::GetDropableItem(string string)
{
	if (string == "itemrope") return Item::ITEMROPE;

	return Item::NONE;
}


