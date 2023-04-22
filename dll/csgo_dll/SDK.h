#pragma once

#include <Windows.h>

struct glow_t
{
	// omitted: unused 4 byte "header"
	DWORD dwBase;
	float r;
	float g;
	float b;
	float a;
	unsigned char unk1[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
	unsigned char unk2[14];
};

struct glow_t_enemy {
	// omitted: DWORD dwBase and 4 byte "header"
	float r = 1.f;
	float g = 0.f;
	float b = 0.f;
	float a = 1.f;
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = true;
	BYTE fullBloom = false;
};

struct glow_t_friend {
	// omitted: DWORD dwBase and 4 byte "header"
	float r = 0.f;
	float g = 0.f;
	float b = 1.f;
	float a = 1.f;
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = true;
	BYTE fullBloom = false;
};