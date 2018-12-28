#include "Sprites.h"
#include <fstream>


CSprites * CSprites::__instance = NULL;

void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
}

void CSprites::AddSpritesFromFile(LPCWSTR filePath, LPDIRECT3DTEXTURE9 tex)
{
	ifstream inputFile(filePath);
	string data = "";
	inputFile >> data;

	// Find the begin of the data
	while (inputFile >> data)
	{
		if (data == "BEGIN")
		{
			int spriteID;
			int left, top, right, bottom;

			// Read the infomation of texture
			while (inputFile >> data)
			{
				if (data == "END")
					break;

				// Get sprite ID
				spriteID = atoi(data.c_str());

				// left
				inputFile >> data;
				left = atoi(data.c_str());

				// top
				inputFile >> data;
				top = atoi(data.c_str());

				// right
				inputFile >> data;
				right = atoi(data.c_str());

				// bottom
				inputFile >> data;
				bottom = atoi(data.c_str());

				this->Add(spriteID, left, top, right, bottom, tex);
			}
		}
	}
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) 
		__instance = new CSprites();
	return __instance;
}
