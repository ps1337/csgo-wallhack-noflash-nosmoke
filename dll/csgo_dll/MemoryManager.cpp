#include "MemoryManager.h"
#include <vector>

MemoryManager::MemoryManager() {
	this->hProc = this->GetProcessHandle("csgo.exe");
	// resolve every module base and save it
	for (auto it = moduleNames.begin(); it != moduleNames.end(); it++) {
		moduleBase.insert(std::pair<std::string, DWORD>(*it, GetModuleBase(*it)));
	}
}

DWORD MemoryManager::GetModuleBase(std::string name)
{
	// cache results
	if (moduleBase.count(name)) {
		return moduleBase[name];
	}

	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, NULL);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	do
		if (!strcmp(mEntry.szModule, name.c_str()))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.modBaseAddr;
		}
	while (Module32Next(hModule, &mEntry));

	throw std::runtime_error("[!] Module not found");
}

HANDLE MemoryManager::GetProcessHandle(std::string moduleName)
{
	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(ProcEntry);
	do
		if (!strcmp(ProcEntry.szExeFile, moduleName.c_str()))
		{
			this->dwPID = ProcEntry.th32ProcessID;
			CloseHandle(hPID);
			return OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
		}
	while (Process32Next(hPID, &ProcEntry));

	throw std::runtime_error("[!] Process not found");
}

DWORD MemoryManager::GetModuleSize(std::string moduleName) {
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, NULL);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	do
		if (!strcmp(mEntry.szModule, moduleName.c_str()))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.modBaseSize;
		}
	while (Module32Next(hModule, &mEntry));

	throw std::runtime_error("[!] Module not found");
}