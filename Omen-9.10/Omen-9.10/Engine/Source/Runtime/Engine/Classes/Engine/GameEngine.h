#pragma once

#include "pch.h"

class GameEngine
{
public:
	static float GetMaxTickRate(UGameEngine* _This, float DeltaTime, bool bAllowFrameRateSmoothing);
};