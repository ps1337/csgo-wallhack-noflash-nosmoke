#pragma once

#include "MemoryManager.h"
#include "SDK.h"

class Base
{
protected:
	static std::string moduleName;
	inline static MemoryManager mem;
};

