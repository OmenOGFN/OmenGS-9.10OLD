#pragma once

#include "pch.h"

class FortGameModeAthena
{
public:
	class Originals
	{
	public:
		inline static bool (*ReadyToStartMatch)(AFortGameModeAthena* _This);
	};
public:
	static bool ReadyToStartMatch(AFortGameModeAthena* _This);
};