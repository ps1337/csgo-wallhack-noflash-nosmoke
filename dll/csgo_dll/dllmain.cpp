#include <Windows.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <ostream>
#include <string>
#include <Psapi.h>
#include <iostream>

#include "offsets.h"
#include "MemoryManager.h"
#include "GlowManager.h"
#include "EntityList.h"
#include "LocalPlayer.h"
#include "SDK.h"
#include "Interface.h"
#include "Haken.h"
#include "PatternScanner.h"

#define DEBUG

HANDLE haxThread;

#ifdef DEBUG
FILE* console;
#endif

/////// Wallhack
void* objClientMode = nullptr;
tPostScreenEffect oPostScreenEffect = nullptr;
// aka DoPostScreenSpaceEffects
void __stdcall hPostScreenEffect(void* edx, void* setup) {
    EntityList::AddGlowToMultiple();
    oPostScreenEffect(objClientMode, edx, setup);
}

unsigned char prologuePostScreenEffect[6];
void InitWallhack() {
    MemoryManager ml;
    void* objClient = (void*)Interface::GetInterface("client.dll", "VClient018");

    // pointer to HudProcessInput method
    // https://www.unknowncheats.me/wiki/Finding_a_pointer_to_ClientMode
    DWORD pHudProcessInput = GetVtableAtIndex<DWORD>(objClient, 10);
    // this points to a pointer to ClientMode
    DWORD ppClientMode = ml.ReadMem<DWORD>((DWORD)pHudProcessInput + 0x5);
    objClientMode = ml.ReadMem<void*>(ppClientMode);
    oPostScreenEffect = (tPostScreenEffect)Haken::hookFn(GetVtableAtIndex<char*>(objClientMode, 44), (char*)hPostScreenEffect, 6, prologuePostScreenEffect, "PostScreenEffect");
}

/////// Material and particle
std::vector<const char*> materials = {
        "particle/vistasmokev1/vistasmokev1_fire",
        "particle/vistasmokev1/vistasmokev1_smokegrenade",
        "particle/vistasmokev1/vistasmokev1_emods",
        "particle/vistasmokev1/vistasmokev1_emods_impactdust",

        "effects\\flashbang",
        "effects\\flashbang_white"
};

unsigned char prologueFrameStateNotify[17];
void* objClient = nullptr;
tFrameStageNotify oFrameStageNotify = nullptr;
tFindMaterial oFindMaterial = nullptr;
void* objMaterialSystem;

void hFrameStageNotify(ClientFrameStage_t curStage) {
    if (curStage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END) {

        for (auto it = materials.begin(); it != materials.end(); it++) {
            auto dasMaterial = oFindMaterial(objMaterialSystem, *it, "Other textures", true, NULL);
            if (!dasMaterial) { continue; }
            GetVtableAtIndex<tSetMaterialVarFlag>(dasMaterial, 29)(dasMaterial, MaterialVarFlags_t::MATERIAL_VAR_NO_DRAW, true);
        }
        oFrameStageNotify(curStage);
    }
    else {
        oFrameStageNotify(curStage);
    }
}

// for noflash and nosmoke
void InitMaterialHax() {
    objClient = (void*)Interface::GetInterface("client.dll", "VClient018");
    objMaterialSystem = (void*)Interface::GetInterface("materialsystem.dll", MATERIAL_SYSTEM_INTERFACE_VERSION);

    // pointer to interface == pointer to the vtable (in this case)
    // --> void*** (pointer to an array of function pointers)
    oFindMaterial = GetVtableAtIndex<tFindMaterial>(objMaterialSystem, 84);
    oFrameStageNotify = (tFrameStageNotify)Haken::hookFn(GetVtableAtIndex<char*>(objClient, 37), (char*)hFrameStageNotify, 17, prologueFrameStateNotify, "FrameStageNotify");
}


void Unhook() {
    if (objClient) {
        Haken::restore(GetVtableAtIndex<char*>(objClient, 37), prologueFrameStateNotify, 17, "FrameStageNotify");
    }

    if (objClientMode) {
        Haken::restore(GetVtableAtIndex<char*>(objClientMode, 44), prologuePostScreenEffect, 6, "PostScreenEffect");
    }
}

DWORD WINAPI Hax(HMODULE hModule)
{
#ifdef DEBUG
    AllocConsole();
    freopen_s(&console, "conout$", "w", stdout);
#endif

    InitMaterialHax();
    InitWallhack();

    while (true)
    {
#ifdef DEBUG
        if (GetAsyncKeyState(VK_DELETE) & 1)
        {
            break;
        }
#endif

        Sleep(200);
    }

#ifdef DEBUG
    fclose(console);
    FreeConsole();
#endif
    Unhook();
    FreeLibraryAndExitThread(hModule, 0);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Hax, hModule, 0, nullptr));
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

