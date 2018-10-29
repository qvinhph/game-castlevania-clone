#pragma once
#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include "Game.h"

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;

public:

	CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	void Draw(float x, float y, ARGB argb = ARGB());

};

typedef CSprite * LPSPRITE;

