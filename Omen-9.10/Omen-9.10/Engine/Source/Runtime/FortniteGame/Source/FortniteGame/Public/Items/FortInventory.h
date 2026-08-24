#pragma once

#include "pch.h"

class FortInventory
{
public:
	static UFortWorldItem* CreateWorldItem(AFortInventory* _This, UFortItemDefinition* ItemDefinition, int32 Count = 1, int32 LoadedAmmo = 0);
	static UFortWorldItem* GetWorldItem(AFortInventory* _This, FGuid& ItemGuid);

	static FFortItemEntry* GetItemEntryByGuid(AFortInventory* _This, FGuid& ItemGuid);
	static FFortItemEntry* GetItemEntryByDefinition(AFortInventory* _This, UFortItemDefinition* ItemDefinition);
	static FFortItemEntry* GetItemEntryByClass(AFortInventory* _This, UClass* Class);

	static void UpdateEntry(AFortInventory* _This, FFortItemEntry* ItemEntry);
	static void RemoveItem(AFortInventory* _This, FGuid& ItemGuid);

	static bool RemoveInventoryItem(IInterface* _This, FGuid& ItemGuid, int32 Count, bool bForceRemoval);
};