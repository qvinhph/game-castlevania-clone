#pragma once
#include <unordered_map>
#include "GameObject.h"
#include "MovableObject.h"


/*
	The class manage all the monster in the game.
	For spawning monsters.
*/
class CMonsters
{
	unordered_map<Monster, vector<LPACTIVEOBJECT>> monsters;

	static CMonsters * __instance;
	CMonsters() = default;

public:

	void Add(Monster monsterName, LPACTIVEOBJECT monster);
	void Add(Monster monsterName, LPGAMEOBJECT monster);

	LPACTIVEOBJECT Spawn(Monster monsterName);
	LPACTIVEOBJECT Spawn(Monster monsterName, float x, float y, int nx);

	static CMonsters * GetInstance();

};

