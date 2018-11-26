#pragma once
#include "GameObject.h"

class CCell;
typedef CCell * LPCELL;

class CCell
{

	vector<LPGAMEOBJECT> objects;

public:
	void Add(LPGAMEOBJECT object);
	void GetObjects(vector<LPGAMEOBJECT> * &objects) { objects = &(this->objects); }
};


