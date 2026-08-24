#include "pch.h"
#include "../../Public/Items/FortInventory.h"
#include "../../Public/Items/FortItemDefinition.h"

#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"

#include "Config.h"

UFortWorldItem* FortInventory::CreateWorldItem(AFortInventory* _This, UFortItemDefinition* ItemDefinition, int32 Count, int32 LoadedAmmo)
{
	if (!ItemDefinition) return nullptr;

	UFortWorldItem* WorldItem = (UFortWorldItem*)ItemDefinition->CreateTemporaryItemInstanceBP(Count, 0);
	AFortPlayerController* PlayerController = (AFortPlayerController*)(_This->GetOwner() ? _This->GetOwner() : _This->GetInstigatorController());

	WorldItem->OwnerInventory = PlayerController->WorldInventory;
	WorldItem->SetOwningControllerForTemporaryItem(PlayerController);
	WorldItem->ItemEntry.ItemDefinition = ItemDefinition;
	WorldItem->ItemEntry.LoadedAmmo = LoadedAmmo;

	_This->Inventory.ItemInstances.Add(WorldItem);
	_This->Inventory.ReplicatedEntries.Add(WorldItem->ItemEntry);
	_This->Inventory.MarkItemDirty(WorldItem->ItemEntry);
	_This->HandleInventoryLocalUpdate();

	return WorldItem;
}

UFortWorldItem* FortInventory::GetWorldItem(AFortInventory* _This, FGuid& ItemGuid)
{
	UFortWorldItem* WorldItem = nullptr;

	for (UFortWorldItem* ItemInstance : _This->Inventory.ItemInstances)
	{
		if (ItemInstance->ItemEntry.ItemGuid == ItemGuid)
		{
			WorldItem = ItemInstance;
			break;
		}
	}

	return WorldItem;
}

FFortItemEntry* FortInventory::GetItemEntryByGuid(AFortInventory* _This, FGuid& ItemGuid)
{
	FFortItemEntry* ItemEntry = nullptr;

	for (FFortItemEntry& ReplicatedEntry : _This->Inventory.ReplicatedEntries)
	{
		if (ReplicatedEntry.ItemGuid == ItemGuid)
		{
			ItemEntry = &ReplicatedEntry;
			break;
		}
	}

	return ItemEntry;
}

FFortItemEntry* FortInventory::GetItemEntryByDefinition(AFortInventory* _This, UFortItemDefinition* ItemDefinition)
{
	FFortItemEntry* ItemEntry = nullptr;

	for (FFortItemEntry& ReplicatedEntry : _This->Inventory.ReplicatedEntries)
	{
		if (ReplicatedEntry.ItemDefinition == ItemDefinition)
		{
			ItemEntry = &ReplicatedEntry;
			break;
		}
	}

	return ItemEntry;
}

FFortItemEntry* FortInventory::GetItemEntryByClass(AFortInventory* _This, UClass* Class)
{
	FFortItemEntry* ItemEntry = nullptr;

	for (FFortItemEntry& ReplicatedEntry : _This->Inventory.ReplicatedEntries)
	{
		if (ReplicatedEntry.ItemDefinition->Class == Class)
		{
			ItemEntry = &ReplicatedEntry;
			break;
		}
	}

	return ItemEntry;
}

void FortInventory::UpdateEntry(AFortInventory* _This, FFortItemEntry* ItemEntry)
{
	if (ItemEntry->bIsReplicatedCopy)
	{
		ItemEntry->bIsDirty = false;

		_This->Inventory.MarkItemDirty(*ItemEntry);
		_This->bRequiresLocalUpdate = true;

		return;
	}

	for (FFortItemEntry& ReplicatedEntry : _This->Inventory.ReplicatedEntries)
	{
		if (ReplicatedEntry.ItemGuid == ItemEntry->ItemGuid)
		{
			ReplicatedEntry = *ItemEntry;
			ReplicatedEntry.bIsDirty = false;

			_This->Inventory.MarkItemDirty(*ItemEntry);
			_This->bRequiresLocalUpdate = true;
			_This->HandleInventoryLocalUpdate();

			if (ItemEntry) _This->Inventory.MarkItemDirty(*ItemEntry);
			else _This->Inventory.MarkArrayDirty();
		}
	}
}

void FortInventory::RemoveItem(AFortInventory* _This, FGuid& ItemGuid)
{
	for (int i = 0; i < _This->Inventory.ItemInstances.Num(); i++)
	{
		if (_This->Inventory.ItemInstances[i]->ItemEntry.ItemGuid == ItemGuid)
		{
			_This->Inventory.ItemInstances.Remove(i);
			break;
		}
	}

	for (int i = 0; i < _This->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (_This->Inventory.ReplicatedEntries[i].ItemGuid == ItemGuid)
		{
			_This->Inventory.ReplicatedEntries.Remove(i);
			break;
		}
	}

	_This->bRequiresLocalUpdate = true;
	_This->HandleInventoryLocalUpdate();
	_This->Inventory.MarkArrayDirty();
}

bool FortInventory::RemoveInventoryItem(IInterface* _This, FGuid& ItemGuid, int32 Count, bool bForceRemoval)
{
	AFortPlayerControllerAthena* PlayerController = (AFortPlayerControllerAthena*)(__int64(_This) - 0x598);
	if (!PlayerController || !PlayerController->WorldInventory || UConfig->bIsCustom1v1Map) return false;

	UFortWorldItem* WorldItem = GetWorldItem(PlayerController->WorldInventory, ItemGuid);
	if (!WorldItem) return false;

	FFortItemEntry* ItemEntry = GetItemEntryByGuid(PlayerController->WorldInventory, ItemGuid);
	if (!ItemEntry) return false;

	ItemEntry->Count -= Count;

	if (Count < 0 || ItemEntry->Count <= 0 || bForceRemoval) { RemoveItem(PlayerController->WorldInventory, ItemGuid); }
	else
	{
		WorldItem->ItemEntry.Count = ItemEntry->Count;

		PlayerController->WorldInventory->Inventory.MarkItemDirty(*ItemEntry);
		PlayerController->WorldInventory->bRequiresLocalUpdate = true;
		PlayerController->WorldInventory->HandleInventoryLocalUpdate();

		return true;
	}

	return true;
}