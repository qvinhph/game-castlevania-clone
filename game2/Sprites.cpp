#include "Sprites.h"

CSprites * CSprites::__instance = NULL;

void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
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
