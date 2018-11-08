#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include "debug.h"

using json = nlohmann::json;

class JSONUtility
{
public:

	/*
		Returns the value mapped by name if it exists and is a JSONArray,
		otherwise return an empty json
	*/
	static json GetObjectArray(json root, std::string name);
	
};

