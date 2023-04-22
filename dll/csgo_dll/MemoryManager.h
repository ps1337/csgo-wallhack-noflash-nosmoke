#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <process.h>
#include <iostream>
#include <vector>
#include <map>

class MemoryManager {
	// todo obfuscate with constexpr
	std::vector<std::string> moduleNames = { "client.dll", "engine.dll" };
	std::map<std::string, DWORD> moduleBase = {};

	HANDLE GetProcessHandle(std::string name);

public:
	DWORD dwPID;
	HANDLE hProc;
	MemoryManager();
	DWORD GetModuleBase(std::string moduleName);
	DWORD GetModuleSize(std::string moduleName);
	
	template <class C>
	C ReadMem(DWORD dwAddress) {
		C result;
		ReadProcessMemory(hProc, (LPCVOID)dwAddress, &result, sizeof(result), NULL);
		return result;
	};

	template <class C>
	BOOL WriteMem(DWORD dwAddress, C value) {
		return WriteProcessMemory(hProc, (LPVOID)dwAddress, &value, sizeof(value), NULL);
	};
};