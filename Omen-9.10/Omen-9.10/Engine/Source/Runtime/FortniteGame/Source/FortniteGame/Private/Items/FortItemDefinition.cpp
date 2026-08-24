#include "pch.h"
#include "../../Public/Items/FortItemDefinition.h"
#include "../../Public/Items/FortWeaponItemDefinition.h"

#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"

FFortItemEntry* FortItemDefinition::MakeItemEntry(UFortItemDefinition* _This, int32 Count, int32 Level)
{
	FFortItemEntry ItemEntry = FFortItemEntry{};
	ItemEntry.MostRecentArrayReplicationKey = -1;
	ItemEntry.ReplicationID = -1;
	ItemEntry.ReplicationKey = -1;
	ItemEntry.ItemDefinition = _This;
	ItemEntry.Count = Count;
	ItemEntry.Level = Level;

	if (UFortWeaponItemDefinition* WeaponItemDefinition = (UFortWeaponItemDefinition*)(_This)) { ItemEntry.LoadedAmmo = FortWeaponItemDefinition::GetRangedWeaponStats(WeaponItemDefinition)->ClipSize; }

	return &ItemEntry;
}

EFortQuickBars FortItemDefinition::GetQuickbar(UFortItemDefinition* _This)
{
	if (!_This) return EFortQuickBars::Max_None;

	return _This->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) || _This->IsA(UFortResourceItemDefinition::StaticClass()) || _This->IsA(UFortAmmoItemDefinition::StaticClass()) || _This->IsA(UFortTrapItemDefinition::StaticClass()) || _This->IsA(UFortBuildingItemDefinition::StaticClass()) || _This->IsA(UFortEditToolItemDefinition::StaticClass()) || ((UFortWorldItemDefinition*)_This)->bForceIntoOverflow ? EFortQuickBars::Secondary : EFortQuickBars::Primary;
}

AFortPickup* FortItemDefinition::SpawnPickup(UFortItemDefinition* _This, int OverrideCount, int LoadedAmmo, FVector Location, EFortPickupSourceTypeFlag PickupSourceTypeFlag, EFortPickupSpawnSource PickupSpawnSource, AFortPawn* Pawn)
{
	auto SpawnedPickup = CoreGlobals::Actors<AFortPickup>(AFortPickup::StaticClass(), Location);
	SpawnedPickup->bRandomRotation = true;

	auto& PickupEntry = SpawnedPickup->PrimaryPickupItemEntry;
	PickupEntry.ItemDefinition = _This;
	PickupEntry.Count = OverrideCount;
	PickupEntry.LoadedAmmo = LoadedAmmo;
	PickupEntry.ReplicationKey++;

	SpawnedPickup->OnRep_PrimaryPickupItemEntry();
	SpawnedPickup->PawnWhoDroppedPickup = Pawn;
	SpawnedPickup->TossPickup(Location, Pawn, -1, true, PickupSourceTypeFlag, PickupSpawnSource);
	SpawnedPickup->SetReplicateMovement(true);
	SpawnedPickup->MovementComponent = (UProjectileMovementComponent*)CoreGlobals::GetDefaultObject<UGameplayStatics>()->SpawnObject(UProjectileMovementComponent::StaticClass(), SpawnedPickup);

	if (PickupSourceTypeFlag == EFortPickupSourceTypeFlag::Container)
	{
		SpawnedPickup->bTossedFromContainer = true;
		SpawnedPickup->OnRep_TossedFromContainer();
	}

	return SpawnedPickup;
}

AFortPickup* FortItemDefinition::SpawnPickup(FFortItemEntry ItemEntry, FVector Location, EFortPickupSourceTypeFlag PickupSourceTypeFlag, EFortPickupSpawnSource PickupSpawnSource)
{
	auto Pickup = SpawnPickup(ItemEntry.ItemDefinition, ItemEntry.Count, ItemEntry.LoadedAmmo, Location, PickupSourceTypeFlag, PickupSpawnSource);
	return Pickup;
}

float FortItemDefinition::GetMaxStackSize(UFortItemDefinition* _This)
{
	if (!_This->MaxStackSize.Curve.CurveTable) return _This->MaxStackSize.Value;
	EEvaluateCurveTableResult Result;
	float Ret;
	((UDataTableFunctionLibrary*)UDataTableFunctionLibrary::StaticClass()->DefaultObject)->EvaluateCurveTableRow(_This->MaxStackSize.Curve.CurveTable, _This->MaxStackSize.Curve.RowName, 0, &Result, &Ret, FString());
	return Ret;
}