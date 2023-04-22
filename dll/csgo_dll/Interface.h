#pragma once

// https://guidedhacking.com/threads/csgo-createinterface-tutorial-how-to-get-interfaces.14701/

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/materialsystem/imaterialsystem.h#L68
#define MATERIAL_SYSTEM_INTERFACE_VERSION "VMaterialSystem080"

class Interface
{
public:
    static void* GetInterface(const char* dllname, const char* interfacename);
};

class IClientEntityList
{
public:
    // Get IClientNetworkable interface for specified entity
    virtual void* GetClientNetworkable(int entnum) = 0;
    virtual void* GetClientNetworkableFromHandle(int hEnt) = 0;
    virtual void* GetClientUnknownFromHandle(int hEnt) = 0;

    // NOTE: This function is only a convenience wrapper.
    // It returns GetClientNetworkable( entnum )->GetIClientEntity().
    virtual void* GetClientEntity(int entnum) = 0;
    virtual void* GetClientEntityFromHandle(int hEnt) = 0;

    // Returns number of entities currently in use
    virtual int                    NumberOfEntities(bool bIncludeNonNetworkable) = 0;

    // Returns highest index actually used
    virtual int                    GetHighestEntityIndex(void) = 0;

    // Sizes entity list to specified size
    virtual void                SetMaxEntities(int maxents) = 0;
    virtual int                    GetMaxEntities() = 0;
};

template<typename Function>Function GetVtableAtIndex(void* _VMT, int Index)
{
    void*** _TVMT = (void***)_VMT;
    void** VMT = *_TVMT;
    void* _Address = VMT[Index];
    return(Function)(_Address);
}

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	//	MATERIAL_VAR_UNUSED					  = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
	//	MATERIAL_VAR_UNUSED					  = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
	MATERIAL_VAR_VERTEXFOG = (1 << 31),

	// NOTE: Only add flags here that either should be read from
	// .vmts or can be set directly from client code. Other, internal
	// flags should to into the flag enum in IMaterialInternal.h
};


enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};


struct DrawModelState_t {
public:
	void* studio_hdr;
	void* studio_hw_data;
	void* renderable;
	const void* model_to_world;
	void*	decals;
	int			drawflags;
	int			lod;
};


class vec3_t {
public:
	float x, y, z;
};

struct ModelRenderInfo_t {
	vec3_t		origin;
	vec3_t		angles;
	char		u0[0x4];
	void* renderable;
	const void* model;
	const void* model_to_world;
	const void* lighting_offset;
	const vec3_t* lighting_origin;
	int		flags;
	int		entity_index;
	int		skin;
	int		body;
	int		hitboxset;
	unsigned short	instance;
};

typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);
typedef void* (__thiscall* tFindMaterial)(void*, char const*, const char*, bool, const char*);
typedef void(__thiscall* tSetMaterialVarFlag)(void*, MaterialVarFlags_t, bool);
typedef void(__thiscall* tFrameStageNotify)(ClientFrameStage_t curStage);
typedef void(__thiscall* tDrawModelExecute)(void*, void*, const DrawModelState_t&, const ModelRenderInfo_t&, void*);
typedef void(__thiscall* tPostScreenEffect)(void* thisptr, void* edx, void* setup);