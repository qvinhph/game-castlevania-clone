#include "GameObject.h"

#include "Textures.h"
#include "Game.h"
#include "Flames.h"
#include "Items.h"
#include "Rope.h"
#include "Board.h"
#include "debug.h"

CCamera * CGameObject::cameraInstance = CCamera::GetInstance();

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
	Return true if 100% area of the object is in the viewport
*/
bool CGameObject::IsInViewport()
{
	float vpLeft, vpRight, vpTop, vpBottom;
	cameraInstance->GetBoundingBox(vpLeft, vpTop, vpRight, vpBottom);
	
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);

	if (vpLeft > left || vpTop > top 
		|| vpRight < right || vpBottom < bottom)
		return false;

	return true;
}

void CGameObject::BeHit(int damage)
{
	this->Destroy();
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

void CGameObject::ReDirect(float positionX)
{
	if		(positionX - x== 0) { /* Do nothing. */}
	else if (positionX - x < 0) nx = -1;
	else	nx = 1;
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
		{
			// Similar to the origin game (only render fully-inside-viewport objects)
			animations->Get(currentAniID)->Render(x, y, argb);
		}	
}

/*
	Add destroying effect 
*/
void CGameObject::Destroy()
{
	CFlames::GetInstance()->ShowAFlame(this);
	CBoard::GetInstance()->AddScore(point);
	this->SetState(STATE_INVISIBLE);
}

CGameObject::CGameObject()
{
	flicker_start = TIMER_IDLE;
	state = STATE_VISIBLE;
	x = y = -1;
	currentAniID = -1;
	lastAniID = -1;

	argb = ARGB();
	animations = CAnimations::GetInstance();
}
