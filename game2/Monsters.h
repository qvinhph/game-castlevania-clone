#pragma once
#include <unordered_map>
#include "GameObject.h"


/*
	The class manage all the monster in the game.
	For spawning monsters.
*/
class CMonsters
{
	unordered_map<Monsters, vector<LPGAMEOBJECT>> monsters;

	static CMonsters * __instance;
	CMonsters() = default;

public:

	void Add(Monsters monsterName, LPGAMEOBJECT monster);
	void Spawn(Monsters monsterName, float x, float y, int nx);			// Spawn the monster at the given point

	static CMonsters * GetInstance();

};

