#include "Animations.h"
#include <fstream>

CAnimations * CAnimations::__instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

void CAnimations::AddAnimationsFromFile(LPCWSTR filePath)
{
	ifstream inputFile(filePath);
	string data = "";
	inputFile >> data;

	// Find the begin of the data
	while (inputFile >> data)
	{
		if (data == "BEGIN")
		{
			int currentAniID;
			int lastAniID = -1;
			int spriteID;		// of a frame
			DWORD frameTime;
			LPANIMATION ani = NULL;

			// Read the infomation of texture
			while (inputFile >> data)
			{
				if (data == "END")
				{
					if (ani != NULL)
						Add(lastAniID, ani);

					break;
				}

				// Get the current animation ID
				currentAniID = atoi(data.c_str());
				if (lastAniID != currentAniID)
				{
					if (ani != NULL)	
						Add(lastAniID, ani);

					lastAniID = currentAniID;
					ani = new CAnimation();
				}

				// Get the spriteID
				inputFile >> data;
				spriteID = atoi(data.c_str());

				// Get the frame time
				inputFile >> data;
				frameTime = atoi(data.c_str());

				// Add frame to animation
				ani->AddFrame(spriteID, frameTime);
			}
		}
	}
}

LPANIMATION CAnimations::Get(int id)
{
	return animations[id];
}
