#include "GameObject.h"

#include "Textures.h"
#include "Game.h"


void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 125);
}

void CGameObject::ResetAnimation(int aniID)
{
	animations->Get(aniID)->SetCurrentFrame(-1);
}

void CGameObject::RenderAnimation(int aniID)
{
	animations->Get(aniID)->Render(x, y);
}

void CGameObject::Destroy()
{
	this->x = OUTSIDE_X;
	this->y = OUTSIDE_Y;
}

CGameObject::CGameObject()
{
	x = y = 0;
	state = 0;
	currentAniID = -1;

	animations = CAnimations::GetInstance();
}
