#pragma once

#include "pch.h"

class FortItemDefinition
{
public:
	static FFortItemEntry* MakeItemEntry(UFortItemDefinition* _This, int32 Count, int32 Level);

	static EFortQuickBars GetQuickbar(UFortItemDefinition* _This);

	static AFortPickup* SpawnPickup(UFortItemDefinition* _This, int OverrideCount, int LoadedAmmo, FVector Location, EFortPickupSourceTypeFlag PickupSourceTypeFlag, EFortPickupSpawnSource PickupSpawnSource, AFortPawn* Pawn = nullptr);
	static AFortPickup* SpawnPickup(FFortItemEntry ItemEntry, FVector Location, EFortPickupSourceTypeFlag PickupSourceTypeFlag = EFortPickupSourceTypeFlag::Other, EFortPickupSpawnSource PickupSpawnSource = EFortPickupSpawnSource::Unset);

	static float GetMaxStackSize(UFortItemDefinition* _This);
};