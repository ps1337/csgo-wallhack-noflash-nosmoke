#pragma once

#include <Windows.h>
#include <string>

class Haken
{
public:
	static void* WINAPI hookFn(char* hookedFn, char* hookFn, int copyBytesSize, unsigned char* backupBytes, std::string descr);
	static BOOL WINAPI restore(char* fn, unsigned char* writeBytes, int writeSize, std::string descr);
};

