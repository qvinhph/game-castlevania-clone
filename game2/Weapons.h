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

	/*
		For keeping track the number of throwable weapons
		can be used at a time.
	*/
	unordered_map<Weapon, int> remainingQuantities;		// current quantity of each weapon

	/* max number of throwable weapon, to increase 
	 this number, get the Double/Tripple Shot item */
	int maxQuantity = 1;


	static CWeapons * __instance;
	CWeapons() = default;

public:

	/*
		Use the given name weapon by the given object ( living things such as: Simon, Fish,..)
	*/
	void UseWeapon(Weapon weaponName, LPGAMEOBJECT obj);

	void UseDagger(const LPGAMEOBJECT &obj);
	void UseFireBall(const LPGAMEOBJECT &obj);
	void UseAxe(const LPGAMEOBJECT &obj);
	void UseHolyWater(const LPGAMEOBJECT &obj);

	/*
		Get a weapon with given name to manipulate with, if it's available
	*/
	LPGAMEOBJECT GetWeapon(Weapon weapon);

	void Add(Weapon weaponName, LPGAMEOBJECT weapon);
	void AddQuantity(Weapon weaponName);

	/*
		Return true if the remaining quantity of the weapon > 0.
		Otherwise, return false.
	*/
	bool CheckQuantity(Weapon weaponName);

	static CWeapons * GetInstance();

};

