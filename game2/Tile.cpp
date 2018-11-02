#include "Tile.h"
#include "Game.h"

CTile::CTile(int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

void CTile::Draw(float x, float y)
{
	CGame * game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom);
}
