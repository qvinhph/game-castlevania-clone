#include "GameObject.h"

#include "Textures.h"
#include "Game.h"
#include "DestroyingFlame.h"
#include "DestroyingFlames.h"

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
	
	ARGB argb = ARGB(100, 255, 255, 255);

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, argb);
}

/*
	Rearrange the frames of the specified animation.
*/
void CGameObject::ResetAnimation(int aniID)
{
	animations->Get(aniID)->SetCurrentFrame(-1);
}

void CGameObject::Render()
{
	animations->Get(currentAniID)->Render(x, y);
}

//void CGameObject::RenderAnimation(int aniID)
//{
//	animations->Get(aniID)->Render(x, y);
//}

void CGameObject::Destroy()
{
	// Get the central point (x , y) of the destroying object
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	float central_x = (l + r) / 2;
	float central_y = (t + b) / 2;
	CDestroyingFlames::GetInstance()->ShowAFlame(central_x, central_y);

	this->x = GRAVEYARD_X;
	this->y = GRAVEYARD_Y;
}

CGameObject::CGameObject()
{
	flickering = false;
	ARGB argb = ARGB();
	x = y = 0;
	state = STATE_INVISIBLE;
	currentAniID = -1;
	animations = CAnimations::GetInstance();
}
