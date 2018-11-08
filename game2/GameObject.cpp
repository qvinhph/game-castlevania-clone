#include "GameObject.h"

#include "Textures.h"
#include "Game.h"
#include "Flames.h"
#include "Items.h"

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
	if (state == STATE_VISIBLE)
		animations->Get(currentAniID)->Render(x, y, argb);
}

/*
	Add destroying effect 
*/
void CGameObject::Destroy()
{
	CFlames::GetInstance()->ShowAFlame(this);
	this->SetState(STATE_INVISIBLE);
}

CGameObject::CGameObject()
{
	flickering = false;
	ARGB argb = ARGB();

	state = STATE_VISIBLE;
	currentAniID = -1;
	animations = CAnimations::GetInstance();
}
