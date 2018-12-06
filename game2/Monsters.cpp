#include "Monsters.h"
#include "debug.h"
#include "Panther.h"
#include "Zombie.h"

CMonsters * CMonsters::__instance = NULL;

void CMonsters::Add(Monster monsterName, LPACTIVEOBJECT monster)
{
	monster->SetState(STATE_INVISIBLE);
	monsters[monsterName].push_back(monster);
}

void CMonsters::Add(Monster monsterName, LPGAMEOBJECT monster)
{
	LPACTIVEOBJECT monsterPtr = dynamic_cast<LPACTIVEOBJECT>(monster);
	this->Add(monsterName, monsterPtr);
}

/*
	Get the pointer to a monster with the given name, if it is avaiable (INVISIBLE)
*/
LPACTIVEOBJECT CMonsters::Spawn(Monster monsterName)
{
	vector<LPACTIVEOBJECT> monstersByName = monsters[monsterName];

	if (monstersByName.empty())
		DebugOut(L"\n[ERROR] No monster with the given name found (Monster enum: %d)", (int)monsterName);

	else
	{
		for (UINT i = 0; i < monstersByName.size(); i++)
			if (monstersByName[i]->GetState() == STATE_INVISIBLE)
				return monstersByName[i];
	}

	return NULL;
}

/*
		Spawn the monster at the given point.
		Then return the pointer to it.
*/
LPACTIVEOBJECT CMonsters::Spawn(Monster monsterName, float x, float y, int nx)
{
	//vector<LPGAMEOBJECT> monstersByName = monsters[monsterName];
	//
	//if (monstersByName.empty())
	//	DebugOut(L"\n[ERROR] No monster with the given name found (Monster enum: %d)", (int)monsterName);
	//
	//else
	//{
	//	for (auto it = monstersByName.begin(); it != monstersByName.end(); ++it)
	//	{
	//		if ((*it)->GetState() == STATE_INVISIBLE)
	//		{
	//			(*it)->SetPosition(x, y);
	//			(*it)->SetDirection(nx);
	//			(*it)->SetState(STATE_VISIBLE);
	//			return *it;
	//		}
	//	}
	//}
	LPACTIVEOBJECT monster = this->Spawn(monsterName);
	if (monster != NULL)
	{
		monster->SetPosition(x, y);
		monster->SetDirection(nx);
		monster->SetState(STATE_VISIBLE);
	}
	
	return monster;
}

CMonsters * CMonsters::GetInstance()
{
	if (__instance == NULL)
		__instance = new CMonsters();

	return __instance;
}
