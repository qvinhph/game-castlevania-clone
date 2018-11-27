#include "Items.h"
#include "debug.h"

CItems * CItems::__instance = NULL;

/*
	Add the given item to the collection according to item name
*/
void CItems::Add(Item itemName, LPGAMEOBJECT item)
{
	item->SetState(STATE_INVISIBLE);
	items[itemName].push_back(item);
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
		Drop(item, x, y);

		
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
void CItems::Drop(Item itemName, float x, float y)
{	
	if (items[itemName].empty())
		DebugOut(L"\n[ERROR] No items with the given name found (Item enum: %d)", (int)itemName);

	else 
	{
		for (auto it = items[itemName].begin(); it != items[itemName].end(); ++it)
		{
			if ((*it)->GetState() == STATE_INVISIBLE)
			{
				(*it)->SetPosition(x, y);
				(*it)->SetState(STATE_VISIBLE);
				break;
			}
		}
	}	
}

CItems * CItems::GetInstance()
{
	if (__instance == NULL)
		__instance = new CItems();

	return __instance;
}
