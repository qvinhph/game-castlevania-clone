#pragma once
#include "Sprite.h"
#include <d3dx9.h>

class CTile
{
	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;

public:

	CTile(int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	void Draw(float x, float y);

};

typedef CTile * LPTILE;

