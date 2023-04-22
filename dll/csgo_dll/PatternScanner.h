#pragma once

#include "MemoryManager.h"
#include <string>

class PatternScanner
{
	MemoryManager mm;
	DWORD size;
	std::unique_ptr<unsigned char[]> data;

public:
	PatternScanner(std::string moduleName);
	DWORD FindPattern(std::string pattern);
};

