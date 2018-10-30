#pragma once
#include "Game.h"
#include "MovableObject.h"

class CFreezeTimeHelper
{
	DWORD freezeTime;

	/*static FreezeHelper * __instance;
	FreezeHelper();*/
public:
	void Freeze(DWORD time);
	//FreezeHelper * GetInstance();
};

