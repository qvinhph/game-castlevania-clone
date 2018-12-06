#include "JsonUtility.h"

json JSONUtility::GetObjectArray(json root, std::string name)
{
	json result = json::array();
	
	if (root[name].is_array() && !(root[name].is_null()))
		result = root[name];
	//else
	//	DebugOut(L"\n[INFO] No json array found with the given key.");
	
	return result;	
}
