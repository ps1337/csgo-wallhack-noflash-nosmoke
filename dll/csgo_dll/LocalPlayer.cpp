#include "LocalPlayer.h"

DWORD LocalPlayer::GetClientModuleBase() {
	return LocalPlayer::mem.GetModuleBase(LocalPlayer::moduleName);
}

DWORD LocalPlayer::GetLocalPlayerBase() {
	return LocalPlayer::mem.ReadMem<DWORD>(LocalPlayer::GetClientModuleBase() + hazedumper::signatures::dwLocalPlayer);
}

int LocalPlayer::GetHealth() {
	return LocalPlayer::mem.ReadMem<int>(GetLocalPlayerBase() + hazedumper::netvars::m_iHealth);
}

float LocalPlayer::GetFlashMaxAlpha() {
	return LocalPlayer::mem.ReadMem<float>(GetLocalPlayerBase() + hazedumper::netvars::m_flFlashMaxAlpha);
}

bool LocalPlayer::SetFlashMaxAlpha() {
	return LocalPlayer::mem.WriteMem<float>(GetLocalPlayerBase() + hazedumper::netvars::m_flFlashMaxAlpha, 0.0);
}

int LocalPlayer::GetTeam() {
	return LocalPlayer::mem.ReadMem<int>(GetLocalPlayerBase() + hazedumper::netvars::m_iTeamNum);
}