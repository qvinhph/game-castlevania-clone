#pragma once
#include "GameObject.h"

class CCell;
typedef CCell * LPCELL;

class CCell
{

	vector<LPGAMEOBJECT> objects;

public:
	void Add(LPGAMEOBJECT object);

	// Return the given vector all the objects of the cell
	void GetObjects(vector<LPGAMEOBJECT> * &objects) { objects = &(this->objects); }
};


