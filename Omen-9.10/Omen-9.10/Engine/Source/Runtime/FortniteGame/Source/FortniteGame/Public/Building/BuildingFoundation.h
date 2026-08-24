#pragma once

#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Stack.h"

class BuildingFoundation
{
public:
	static void ShowFoundation(ABuildingFoundation* _This);
	static void SetDynamicFoundationEnabled(ABuildingFoundation* _This, FFrame& Stack);
	static void SetDynamicFoundationTransform(ABuildingFoundation* _This, FFrame& Stack);
};