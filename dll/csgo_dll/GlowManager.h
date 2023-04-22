#pragma once

#include "MemoryManager.h"
#include "Base.h"
#include "offsets.h"
#include <vector>

class GlowManager : Base
{
	inline static DWORD base = mem.ReadMem<DWORD>((DWORD)(mem.GetModuleBase(moduleName) + hazedumper::signatures::dwGlowObjectManager));
	inline static int elementOffset = sizeof glow_t;
	inline static int listSize = 64;
	// skip trash at start of element
	inline static int skipBytes = 0x4;
public:
	static glow_t* GetGlowElement(int glowIdx);
	static DWORD GetGlowElementAddr(int glowIdx);
	static std::vector<DWORD> GetAll();
	static bool AddGlow(int glowIdx, bool isEnemy=true);
	static bool AddGlow(DWORD addr, bool isEnemy=true);
};

