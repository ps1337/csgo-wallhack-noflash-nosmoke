#pragma once

#include "MemoryManager.h"
#include "offsets.h"
#include "Base.h"
#include <string>

class LocalPlayer : Base
{
	static DWORD GetClientModuleBase();
public:
	static DWORD GetLocalPlayerBase();
	static int GetHealth();
	static int GetTeam();
	static float GetFlashMaxAlpha();
	// alternative noflash
	static bool SetFlashMaxAlpha();
};

