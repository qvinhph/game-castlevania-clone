#pragma once
#include <unordered_map>

#include "Dagger.h"
#include "MovableObject.h"

using namespace std;

/*
	A class help managing all the weapon that can be throwed or used
	by living objects (such as Simon, Fish,...)
*/
class CWeapons
{
	unordered_map<Weapon, vector<LPGAMEOBJECT>> weapons;
	unordered_map<Weapon, int> stock;		// current quantity of each weapon

	static CWeapons * __instance;
	CWeapons() = default;

public:

	/*
		Use the given name weapon by the given object ( living things such as: Simon, Fish,..)
	*/
	void UseWeapon(Weapon weaponName, LPGAMEOBJECT obj);

	void UseDagger(const LPGAMEOBJECT &obj);
	void UseFireBall(const LPGAMEOBJECT &obj);

	/*
		Get a weapon with given name to manipulate with, if it's available
	*/
	LPGAMEOBJECT GetWeapon(Weapon weapon);

	void Add(Weapon weaponName, LPGAMEOBJECT weapon);
	void AddToStock(Weapon weaponName);

	static CWeapons * GetInstance();

};

