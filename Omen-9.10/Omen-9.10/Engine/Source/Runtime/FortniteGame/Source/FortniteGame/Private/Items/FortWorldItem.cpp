#include "pch.h"
#include "../../Public/Items/FortWorldItem.h"
#include "../../Public/Items/FortInventory.h"

void FortWorldItem::SetLoadedAmmo(UFortWorldItem* _This, int32 LoadedAmmo)
{
	AFortPlayerControllerAthena* PlayerController = (AFortPlayerControllerAthena*)(_This->GetOwningController());
	if (!PlayerController) return;

	FFortItemEntry* ItemEntry = FortInventory::GetItemEntryByGuid(PlayerController->WorldInventory, _This->ItemEntry.ItemGuid);
	if (!ItemEntry) return;

	ItemEntry->LoadedAmmo = LoadedAmmo;

	_This->ItemEntry.LoadedAmmo = LoadedAmmo;

	PlayerController->WorldInventory->Inventory.MarkItemDirty(*ItemEntry);
	PlayerController->WorldInventory->bRequiresLocalUpdate = true;
	PlayerController->WorldInventory->HandleInventoryLocalUpdate();
}