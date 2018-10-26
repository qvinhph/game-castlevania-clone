#pragma once
#pragma once
#include <Windows.h>
#include <d3dx9.h>

class CSprite
{
	// DEBUGGING int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;

public:
	int id;

	CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	void Draw(float x, float y, int alpha = 255);

};

typedef CSprite * LPSPRITE;

