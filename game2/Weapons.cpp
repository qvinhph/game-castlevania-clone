#include "Weapons.h"
#include "debug.h"
#include "Simon.h"
#include "Fish.h"
#include "Timer.h"

// To adjusting the Dagger position for similar to origin game
#define Y_DISTANCE_DAGGER_LOWER_SIMON			4.0f

// To adjusting the Fireball position for similar to origin game
#define Y_DISTANCE_FIREBALL_LOWER_FISH			4.0f

#define DOUBLESHOT_MAX_QUANTITY		2
#define TRIPLESHOT_MAX_QUANTITY		3

CWeapons * CWeapons::__instance = NULL;

void CWeapons::UseWeapon(Weapon weaponName, LPGAMEOBJECT obj)
{
	switch (weaponName)
	{
	case Weapon::DAGGER:
		UseDagger(obj);
		break;
	case Weapon::FIREBALL:
		UseFireBall(obj);
		break;
	case Weapon::AXE:
		UseAxe(obj);
		break;
	case Weapon::HOLYWATER:
		UseHolyWater(obj);
		break;
	default:
		break;
	}
}

void CWeapons::UseDagger(const LPGAMEOBJECT &obj)
{
	// Only simon can use this dagger weapon.
	if (dynamic_cast<CSimon *>(obj))
	{
		LPGAMEOBJECT weapon = GetWeapon(Weapon::DAGGER);

		if (weapon == NULL)
		{
			DebugOut(L"\n[ERROR] Failed to get dagger weapon");
			return;
		}


		CSimon * simon = dynamic_cast<CSimon *>(obj);
		float left, top, right, bottom;

		// Get the direction depending on Simon's direction
		weapon->SetDirection(simon->GetDirection());
		simon->GetBoundingBox(left, top, right, bottom);

		// Set the weapon position depending on Simon's position
		if (weapon->GetDirection() > 0)
			weapon->SetPosition(right,
				top + Y_DISTANCE_DAGGER_LOWER_SIMON);
		else
			weapon->SetPosition(left - DAGGER_BBOX_WIDTH,
				top + Y_DISTANCE_DAGGER_LOWER_SIMON);

		// Set the weapon visible
		weapon->SetState(STATE_VISIBLE);
	}
}

void CWeapons::UseFireBall(const LPGAMEOBJECT & obj)
{
	// Only simon can use this dagger weapon.
	if (dynamic_cast<CFish *>(obj))
	{
		LPGAMEOBJECT weapon = GetWeapon(Weapon::FIREBALL);

		if (weapon == NULL)
		{
			DebugOut(L"\n[ERROR] Failed to get fireball");
			return;
		}


		CFish * fish = dynamic_cast<CFish *>(obj);
		float xFish, yFish;
		fish->GetPosition(xFish, yFish);

		// Change nx
		weapon->SetDirection(fish->GetDirection());

		// Set the position
		weapon->SetPosition(xFish, yFish + Y_DISTANCE_FIREBALL_LOWER_FISH);

		// Set the weapon visible
		weapon->SetState(STATE_VISIBLE);
	}
}

void CWeapons::UseAxe(const LPGAMEOBJECT & obj)
{
	// Only simon can use this axe weapon.
	if (dynamic_cast<CSimon *>(obj))
	{
		CSimon * simon = dynamic_cast<CSimon *>(obj);
		float left, top, right, bottom;

		LPGAMEOBJECT weapon = GetWeapon(Weapon::AXE);

		if (weapon == NULL)
		{
			DebugOut(L"\n[ERROR] Failed to get dagger weapon");
			return;
		}

		// Get the direction depending on Simon's direction
		weapon->SetDirection(simon->GetDirection());
		simon->GetBoundingBox(left, top, right, bottom);

		// Set the weapon position depending on Simon's position
		// Axe will be appear above simon's head
		weapon->SetPosition((left + right) / 2, top);

		// Set the weapon visible
		weapon->SetState(STATE_VISIBLE);
	}
}

void CWeapons::UseHolyWater(const LPGAMEOBJECT & obj)
{
	// Only simon can use this holy water weapon.
	if (dynamic_cast<CSimon *>(obj))
	{
		LPGAMEOBJECT weapon = GetWeapon(Weapon::HOLYWATER);

		if (weapon == NULL)
		{
			DebugOut(L"\n[ERROR] Failed to get dagger weapon");
			return;
		}


		CSimon * simon = dynamic_cast<CSimon *>(obj);
		float left, top, right, bottom;

		// Get the direction depending on Simon's direction
		weapon->SetDirection(simon->GetDirection());
		simon->GetBoundingBox(left, top, right, bottom);

		// Set the weapon position depending on Simon's position
		// Holy water will be appear above simon's head
		weapon->SetPosition((left + right) / 2, top);

		// Set the weapon visible
		weapon->SetState(STATE_VISIBLE);
	}
}

LPGAMEOBJECT CWeapons::GetWeapon(Weapon weaponName)
{
	vector<LPGAMEOBJECT> weaponsByName = weapons[weaponName];
	int * quantityOfWeapon = &remainingQuantities[weaponName];

	// Check if any weapon is available
	if (*quantityOfWeapon > 0)
	{
		for (auto it = weaponsByName.begin(); it != weaponsByName.end(); ++it)
		{
			if ((*it)->GetState() == STATE_INVISIBLE)
			{
				(*quantityOfWeapon)--;
				return *it;
			}
		}
	}
	else
		DebugOut(L"\n[INFO] Out of stock of the weapon (Weapon enum: %d)", (int)weaponName);
	
	return NULL;
}

void CWeapons::Add(Weapon weaponName, LPGAMEOBJECT weapon)
{
	weapon->SetState(STATE_INVISIBLE);
	weapons[weaponName].push_back(weapon);
	remainingQuantities[weaponName] = maxQuantity;
}

void CWeapons::AddQuantity(Weapon weaponName)
{
	if (remainingQuantities[weaponName] < maxQuantity)
		remainingQuantities[weaponName]++;
}

void CWeapons::UpgradeToDoubleShot()
{
	maxQuantity = DOUBLESHOT_MAX_QUANTITY;
	for (auto it = remainingQuantities.begin(); it != remainingQuantities.end(); ++it)
	{
		(*it).second = maxQuantity;
	}		
}

bool CWeapons::CheckQuantity(Weapon weaponName)
{
	if (remainingQuantities[weaponName] > 0)
		return true;

	return false;
}

CWeapons * CWeapons::GetInstance()
{
	if (__instance == NULL)
		__instance = new CWeapons();

	return __instance;
}
