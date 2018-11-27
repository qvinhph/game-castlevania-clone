#include "Monsters.h"
#include "debug.h"
#include "Panther.h"
#include "Zombie.h"

CMonsters * CMonsters::__instance = NULL;

void CMonsters::Add(Monsters monsterName, LPGAMEOBJECT monster)
{
	monster->SetState(STATE_INVISIBLE);
	monsters[monsterName].push_back(monster);
}

void CMonsters::Spawn(Monsters monsterName, float x, float y, int nx)
{
	vector<LPGAMEOBJECT> monstersByName = monsters[monsterName];

	if (monstersByName.empty())
		DebugOut(L"\n[ERROR] No monster with the given name found (Monster enum: %d)", (int)monsterName);

	else
	{
		for (auto it = monstersByName.begin(); it != monstersByName.end(); ++it)
		{
			if ((*it)->GetState() == STATE_INVISIBLE)
			{
				(*it)->SetPosition(x, y);
				(*it)->SetDirection(nx);
				(*it)->SetState(STATE_VISIBLE);
				return;
			}
		}
	}
}

CMonsters * CMonsters::GetInstance()
{
	if (__instance == NULL)
		__instance = new CMonsters();

	return __instance;
}
