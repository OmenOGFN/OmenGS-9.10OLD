#pragma once

#include "pch.h"
#include "EngineBaseTypes.h"

inline __int64 (*CantBuild)(UWorld*, UObject*, FVector, FRotator, char, void*, char*) = decltype(CantBuild)(InSDKUtils::GetImageBase() + 0x144D670);

class World
{
public:
	static ENetMode GetNetMode(UWorld* _This);

	static bool ListenCall(UWorld* _This);
};