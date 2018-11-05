#include "Items.h"
#include "debug.h"

CItems * CItems::__instance = NULL;

void CItems::Add(Item itemName, LPGAMEOBJECT itemObj)
{
	items[itemName].push_back(itemObj);
}

void CItems::CheckAndDrop(LPGAMEOBJECT object)
{
	Item item = object->GetHoldingItem();

	if (item != Item::NONE)
	{
		float x, y;
		object->GetPosition(x, y);
		GetAndSet(item, x, y);

		
		// These code may useful for choosing what to drop
		// According to Simon stats ( rope level, ...)
		/*switch (item)
		{
		case Item::ITEMROPE:
			GetAndSet(item, x, y);
		case Item::HEART:
		defautl:
			break;
		}*/
	}	
}

void CItems::GetAndSet(Item itemName, float x, float y)
{
	vector<LPGAMEOBJECT> itemsByName = items[itemName];
	
	if (itemsByName.empty())
	{
		DebugOut(L"\n[ERROR] No items with the given name found");
		return;
	}

	for (auto it = itemsByName.begin(); it != itemsByName.end(); ++it)
	{
		if ((*it)->GetState() == STATE_INVISIBLE)
		{
			(*it)->SetPosition(x, y);
			(*it)->SetState(STATE_VISIBLE);
			return;
		}
	}
}

CItems * CItems::GetInstance()
{
	if (__instance == NULL)
		__instance = new CItems();

	return __instance;
}
