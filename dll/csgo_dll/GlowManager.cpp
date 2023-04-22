#include "GlowManager.h"

glow_t* GlowManager::GetGlowElement(int glowIndex) {
	DWORD dwGlowElem = base + (glowIndex * elementOffset) + skipBytes;
	return (glow_t*)dwGlowElem;
}

DWORD GlowManager::GetGlowElementAddr(int glowIndex) {
	return (base + (glowIndex * sizeof glow_t)) + skipBytes;
}

bool GlowManager::AddGlow(int glowIndex, bool isEnemy) {
	return GlowManager::AddGlow(GlowManager::GetGlowElementAddr(glowIndex), isEnemy);
}

bool GlowManager::AddGlow(DWORD addr, bool isEnemy) {
	// skip dwBase entry
	addr += sizeof DWORD;

	if (isEnemy) {
		glow_t_enemy glowData;
		return mem.WriteMem<glow_t_enemy>(addr, glowData);
	}
	else {
		glow_t_friend glowData;
		return mem.WriteMem<glow_t_friend>(addr, glowData);
	}
}

std::vector<DWORD> GlowManager::GetAll() {
	std::vector<DWORD> result;
	for (int i = 0; i < listSize; i++) {
		result.push_back(GlowManager::GetGlowElementAddr(i));
	}
	return result;
}