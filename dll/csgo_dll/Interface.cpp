#include "Interface.h"
#include <Windows.h>
#include <stdexcept>

// https://guidedhacking.com/threads/csgo-createinterface-tutorial-how-to-get-interfaces.14701/
void* Interface::GetInterface(const char* dllname, const char* interfacename)
{
    tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllname), "CreateInterface");

    int returnCode = 0;
    void* result = CreateInterface(interfacename, &returnCode);
    if (!result) {
        throw std::runtime_error("[!] Could not create interface");
    }
    return result;
}