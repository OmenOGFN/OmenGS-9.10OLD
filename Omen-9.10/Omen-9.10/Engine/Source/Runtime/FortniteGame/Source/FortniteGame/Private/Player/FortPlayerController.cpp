#include "pch.h"
#include "../../Public/Player/FortPlayerController.h"
#include "../../Public/Items/FortInventory.h"

#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"

#include "Config.h"

void FortPlayerController::ServerExecuteInventoryItem(AFortPlayerController* _This, FGuid& ItemGuid)
{
	AFortPlayerPawnAthena* PlayerPawn = Cast<AFortPlayerPawnAthena>(_This->MyFortPawn);
	if (!PlayerPawn) return;

	FFortItemEntry* ItemEntry = FortInventory::GetItemEntryByGuid(_This->WorldInventory, ItemGuid);
	if (!ItemEntry) return;

	UFortWeaponItemDefinition* WeaponItemDefinition = Cast<UFortWeaponItemDefinition>(ItemEntry->ItemDefinition);
	if (!WeaponItemDefinition) return;

	if (UFortContextTrapItemDefinition* ContextTrapItemDefinition = Cast<UFortContextTrapItemDefinition>(WeaponItemDefinition))
	{
		PlayerPawn->PickUpActor(nullptr, ContextTrapItemDefinition);
		PlayerPawn->CurrentWeapon->ItemEntryGuid = ItemGuid;

		if (AFortDecoTool_ContextTrap* DecoTool_ContextTrap = Cast<AFortDecoTool_ContextTrap>(PlayerPawn->CurrentWeapon)) { DecoTool_ContextTrap->ContextTrapItemDefinition = ContextTrapItemDefinition; }
	}
	else { PlayerPawn->EquipWeaponDefinition(WeaponItemDefinition, ItemGuid); }
}