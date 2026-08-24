#pragma once

#include <Windows.h>

class Config
{
public:
	inline static bool bIsCreative = false;
	inline static bool bIsCustom1v1Map = false;
	inline static bool bIsLateGame = false;
	inline static bool bIsArenaSolo = false;
	inline static bool bIsArenaDuos = false;
	inline static bool bIsInfiniteMaterials = false;
};
inline Config* UConfig = NULL;