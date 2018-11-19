#include "GameObject.h"

#include "Textures.h"
#include "Game.h"
#include "Flames.h"
#include "Items.h"
#include "debug.h"


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
	Return true if 100% of the object area in the viewport
*/
bool CGameObject::IsInViewport()
{
	float xCam, yCam;
	float viewportWidth, viewportHeight;
	CGame::GetInstance()->GetCameraPosition(xCam, yCam);
	CGame::GetInstance()->GetViewportSize(viewportWidth, viewportHeight);
	
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);

	if (xCam > left || yCam > top ||
		xCam + viewportWidth < right ||
		yCam + viewportHeight < bottom)
		return false;
	//if (0 > left || 0 > top ||
	//	viewportWidth < right ||
	//	viewportHeight < bottom)
	//	return false;

	return true;
}

void CGameObject::FreezeAnimation()
{
	if (currentAniID != -1)
		animations->Get(currentAniID)->ResetFrameStartTime();
}

/*
	Rearrange the frames of the given animation.
*/
void CGameObject::ResetAnimation(int aniID)
{
	animations->Get(aniID)->SetCurrentFrame(-1);
}

void CGameObject::Render()
{
	if (state == STATE_VISIBLE)
		if (this->IsInViewport())
			// Similar to the origin game (only render fully-inside-viewport objects)
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
	flicker_start = 0;
	ARGB argb = ARGB();

	state = STATE_VISIBLE;
	currentAniID = -1;
	animations = CAnimations::GetInstance();
}
