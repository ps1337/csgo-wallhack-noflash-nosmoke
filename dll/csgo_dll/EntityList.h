#pragma once

#include "Base.h"
#include "offsets.h"
#include "GlowManager.h"
#include "LocalPlayer.h"
#include <vector>
#include <map>

class EntityList : Base
{
	inline static DWORD base = mem.GetModuleBase(moduleName) + hazedumper::signatures::dwEntityList;
	inline static int elementOffset = 0x10;
	inline static int listSize = 32;
	inline static std::vector<DWORD> playerEntities;
public:
	static void Init();
	static DWORD GetPlayerEntity(int idx);
	static std::vector<DWORD> GetAllPlayers();
	static int GetTeam(int idx);
	static int GetGlowIndex(int idx);
	static std::map<int, int> GetAllPlayerGlowIndexes();
	static glow_t* GetGlowElement(int idx);
	static DWORD GetGlowElementAddr(int idx);
	// wallhack
	static bool AddGlowToMultiple(bool enemiesOnly=true);
	static bool IsEnemy(int idx);
};

