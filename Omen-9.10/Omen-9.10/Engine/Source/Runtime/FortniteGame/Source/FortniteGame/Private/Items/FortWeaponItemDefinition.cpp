#include "pch.h"
#include "../../Public/Items/FortWeaponItemDefinition.h"

FFortRangedWeaponStats* FortWeaponItemDefinition::GetRangedWeaponStats(UFortWeaponItemDefinition* _This)
{
	if (!_This || !_This->WeaponStatHandle.DataTable) return nullptr;

	for (const auto& [RowName, RowValue] : _This->WeaponStatHandle.DataTable->RowMap)
	{
		FFortRangedWeaponStats* RangedWeaponStats = (FFortRangedWeaponStats*)RowValue;
		if (!RangedWeaponStats) continue;

		if (_This->WeaponStatHandle.RowName == RowName) { return RangedWeaponStats; }
	}

	return nullptr;
}