#include "algorithm"

#include "MovableObject.h"
#include "Game.h"
#include "ItemRope.h"
#include "InvisibleWall.h"

CMovableObject::CMovableObject()
{
	vx = 0;
	vy = 0;
	nx = 1;
}

void CMovableObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	this->dx = vx * dt;
	this->dy = vy * dt;
}


void CMovableObject::ProceedCollisions(vector<LPCOLLISIONEVENT>& coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;
	float min_tx, min_ty, nx, ny;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	y += min_ty * dy;
	x += min_tx * dx;

	for (UINT i = 0; i < coEventsResult.size(); ++i)
	{
		if (dynamic_cast<CInvisibleWall *>(coEventsResult[i]->obj))
		{
			// Block when reach the ground
			if (ny < 0)
			{
				vy = 0;
				y += DEFLECTION_AVOID_OVERLAPPING * ny;
			}
		}
	}
}

void CMovableObject::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == STATE_INVISIBLE)
		vx = vy = dx = dy = 0;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CMovableObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object if it is: m speed = original m speed - collide object speed
	float svx = 0;
	float svy = 0;
	if (dynamic_cast<CMovableObject *>(coO))
		dynamic_cast<CMovableObject *>(coO)->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent * e = new CCollisionEvent(t, nx, ny, coO);
	return e;
}

void CMovableObject::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty,
	float &nx, float &ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CMovableObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

