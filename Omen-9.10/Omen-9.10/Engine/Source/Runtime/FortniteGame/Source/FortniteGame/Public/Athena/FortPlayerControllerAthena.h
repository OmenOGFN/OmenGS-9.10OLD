#pragma once

#include "pch.h"

class FortPlayerControllerAthena
{
public:
	class Originals
	{
	public:
		inline static void (*ServerPlayEmoteItem)(AFortPlayerControllerAthena* _This, UFortMontageItemDefinitionBase* MontageItemDefinitionBase);
	};
public:
	static void ServerPlayEmoteItem(AFortPlayerControllerAthena* _This, UFortMontageItemDefinitionBase* MontageItemDefinitionBase);
};