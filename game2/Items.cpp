#include "Items.h"
#include "debug.h"

CItems * CItems::__instance = NULL;

/*
	Add the given item to the collection according to item name
*/
void CItems::Add(Item itemName, LPGAMEOBJECT itemObj)
{
	itemObj->SetState(STATE_INVISIBLE);
	items[itemName].push_back(itemObj);
}

/*
	Check if the gameobject is holding item, drop it if has
*/
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

/*
	Get the item according to the given name and set it the gien position
*/
void CItems::GetAndSet(Item itemName, float x, float y)
{
	vector<LPGAMEOBJECT> itemsByName = items[itemName];
	
	if (itemsByName.empty())
	{
		DebugOut(L"\n[ERROR] No items with the given name (order in enum: %d) found", (int)itemName);
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
