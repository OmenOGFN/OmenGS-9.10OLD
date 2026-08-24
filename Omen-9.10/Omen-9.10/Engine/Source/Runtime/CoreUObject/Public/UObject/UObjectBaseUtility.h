#pragma once

#include "pch.h"

class UObjectBaseUtility
{
public:
	template <typename _It>
	inline static _It* GetInterfaceAddress(UObject* Object) { return ((_It * (*)(UObject*, UClass*)) (InSDKUtils::GetImageBase() + 0x1FF3650))(Object, _It::StaticClass()); }
};