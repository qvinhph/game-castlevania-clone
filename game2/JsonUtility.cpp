#include "JsonUtility.h"

json JSONUtility::GetObjectArray(json root, std::string name)
{
	json result = json::array();
	
	if (root[name].is_array() && !(root[name].is_null()))
		result = root[name];
	else
		// TODO: FIX THIS. Output doesn't meet out expectation.
		DebugOut(L"\n[INFO] No json array found with key = %s", name.c_str());
	
	return result;	
}
