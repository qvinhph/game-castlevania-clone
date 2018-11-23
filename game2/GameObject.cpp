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

bool CGameObject::IsOverlapping(LPGAMEOBJECT obj)
{
	float left, top, right, bottom;
	float leftObj, topObj, rightObj, bottomObj;

	obj->GetBoundingBox(leftObj, topObj, rightObj, bottomObj);
	GetBoundingBox(left, top, right, bottom);

	if (left < rightObj && right > leftObj &&
		top < bottomObj && bottom > topObj)
		return true;

	return false;
}

void CGameObject::ReDirect(float x)
{
	if (this->x - x == 0) return;
	nx = ((this->x - x) > 0) ? -1 : 1;
}

void CGameObject::FreezeAnimation()
{
	if (currentAniID != -1)
		animations->Get(currentAniID)->ResetFrameStartTime();
}

/*
	Rearrange the frames of the given animation.
*/
void CGameObject::ResetAnimationTimer(int aniID)
{
	animations->Get(aniID)->SetCurrentFrame(-1);
}

void CGameObject::Render()
{
	// Check if stop using an animation
	// If yes, put the frames of the animation in order (again)
	if (currentAniID != lastAniID)
		if (lastAniID != -1)
			this->ResetAnimationTimer(lastAniID);

	lastAniID = currentAniID;

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
