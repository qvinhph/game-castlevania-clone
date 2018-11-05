#include "GameObject.h"

#include "Textures.h"
#include "Game.h"
#include "DestroyingFlames.h"
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

void CGameObject::SetState(int state)
{
	this->state = state;

	// Send the object to invisible zone
	if (state == STATE_INVISIBLE)
		SetPosition(INSIVIBLE_ZONE_X, INSIVIBLE_ZONE_Y);
}

void CGameObject::Render()
{
	animations->Get(currentAniID)->Render(x, y);
}

void CGameObject::Destroy()
{
	// Get the central point (x , y) of the destroying object
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	float central_x = (l + r) / 2;
	float central_y = (t + b) / 2;
	CDestroyingFlames::GetInstance()->ShowAFlame(central_x, central_y);

	// Drop the item the object is holding, if has
	CItems::GetInstance()->CheckAndDrop(this);

	this->x = INSIVIBLE_ZONE_X;
	this->y = INSIVIBLE_ZONE_Y;
}

CGameObject::CGameObject()
{
	x = INSIVIBLE_ZONE_X;
	y = INSIVIBLE_ZONE_Y;

	flickering = false;
	ARGB argb = ARGB();

	state = STATE_INVISIBLE;
	currentAniID = -1;
	animations = CAnimations::GetInstance();
}
