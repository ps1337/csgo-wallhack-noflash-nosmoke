#include "EntityList.h"

// Note: We could some of this with the Interface method, e.g.
// IClientEntityList* ClientEntityList = (IClientEntityList*)Interface::GetInterface("client.dll", "VClientEntityList003");
// But this code works and I don't want to change it, ok?

DWORD EntityList::GetPlayerEntity(int entityListIdx) {
	if (entityListIdx > 32) {
		throw std::runtime_error("[!] EntiyList: Invalid Index");
	}

	DWORD result = mem.ReadMem<DWORD>(base + (entityListIdx * elementOffset));
	return result;
}

int EntityList::GetTeam(int entityListIdx) {
	auto dwEnt = GetPlayerEntity(entityListIdx);
	if (!dwEnt) { return -1; }
	return mem.ReadMem<int>(dwEnt + hazedumper::netvars::m_iTeamNum);
}

int EntityList::GetGlowIndex(int entityListIdx) {
	auto dwEnt = GetPlayerEntity(entityListIdx);
	if (!dwEnt) { return -1; }
	return mem.ReadMem<int>(dwEnt + hazedumper::netvars::m_iGlowIndex);
}

glow_t* EntityList::GetGlowElement(int entityListIdx) {
	int glowIdx = EntityList::GetGlowIndex(entityListIdx);
	if (!glowIdx) { return nullptr; }
	return GlowManager::GetGlowElement(glowIdx);
}

DWORD EntityList::GetGlowElementAddr(int entityListIdx) {
	int glowIdx = EntityList::GetGlowIndex(entityListIdx);
	if (!glowIdx) { return -1; }
	return GlowManager::GetGlowElementAddr(glowIdx);
}

std::vector<DWORD> EntityList::GetAllPlayers() {
	std::vector<DWORD> result;
	for (int i = 0; i < listSize; i++) {
		DWORD ent = EntityList::GetPlayerEntity(i);
		if (!ent) { continue; }
		result.push_back(ent);
	}
	return result;
}

std::map<int, int> EntityList::GetAllPlayerGlowIndexes() {
	std::map<int, int> result;
	auto allEntities = EntityList::GetAllPlayers();
	for (auto it = allEntities.begin(); it != allEntities.end(); it++) {
		bool isDead = mem.ReadMem<bool>(*it + hazedumper::signatures::m_bDormant);
		if (isDead) { continue; }
		auto curGlowIdx = mem.ReadMem<int>(*it + hazedumper::netvars::m_iGlowIndex);
		// create a map: {e : glowIdx}
		auto elem = std::pair<int, int>(it - allEntities.begin(), curGlowIdx);
		result.insert(elem);
	}
	return result;
}

bool EntityList::IsEnemy(int idx) {
	return LocalPlayer::GetTeam() != EntityList::GetTeam(idx);
}

bool EntityList::AddGlowToMultiple(bool enemiesOnly) {
	auto allGlowIndexes = EntityList::GetAllPlayerGlowIndexes();
	for (auto it = allGlowIndexes.begin(); it != allGlowIndexes.end(); it++) {
		if (!enemiesOnly || (enemiesOnly && IsEnemy(it->first))) {
			GlowManager::AddGlow(it->second);
		}
	}
}