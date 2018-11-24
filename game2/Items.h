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
	
	void Add(Item itemName, LPGAMEOBJECT item);			
	void CheckAndDrop(LPGAMEOBJECT gameObject);
	void GetAndShowItem(Item iteamName, float x, float y);	

	static CItems * GetInstance();
};

