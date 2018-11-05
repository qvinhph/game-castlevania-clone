#pragma once
#include <unordered_map>
#include "GameObject.h"

using namespace std;

class CItems
{
	unordered_map<Item, vector<LPGAMEOBJECT>> items;

	static CItems * __instance;
	CItems() = default;

public:
	
	void Add(Item itemName, LPGAMEOBJECT item);			// Add the given item to the collection 
														// according to item name

	void CheckAndDrop(LPGAMEOBJECT gameObject);			// Check if the gameobject is holding item,
														// drop it if has

	void GetAndSet(Item iteamName, float x, float y);	// Get the item according to the given name 
														// and set its position

	static CItems * GetInstance();

};

