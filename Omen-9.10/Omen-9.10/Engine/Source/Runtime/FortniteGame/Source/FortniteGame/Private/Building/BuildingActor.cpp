#include "pch.h"
#include "../../Public/Building/BuildingActor.h"
#include "../../Public/Items/FortItemDefinition.h"

void BuildingActor::OnDamageServer(ABuildingActor* _This, float Damage, FGameplayTagContainer& DamageTags, FVector& Momentum, FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle& EffectContext)
{
	if (!_This || !InstigatedBy || !InstigatedBy->IsA(AFortPlayerControllerAthena::StaticClass()) || !DamageCauser->IsA(AFortWeapon::StaticClass()) || !((AFortWeapon*)DamageCauser)->WeaponData->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) || _This->bPlayerPlaced) return Originals::OnDamageServer(_This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

	ABuildingSMActor* BuildingSMActor = (ABuildingSMActor*)_This;
	AFortPlayerControllerAthena* PlayerController = (AFortPlayerControllerAthena*)InstigatedBy;

	if (!PlayerController->Pawn) return Originals::OnDamageServer(_This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

	AFortPlayerPawnAthena* PlayerPawn = (AFortPlayerPawnAthena*)PlayerController->Pawn;

	int MaterialCount = (Damage / (UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(4, 8)));

	PlayerController->ClientReportDamagedResourceBuilding(BuildingSMActor, BuildingSMActor->ResourceType, MaterialCount, BuildingSMActor->bDestroyed, (Damage == 100.f));

	UFortResourceItemDefinition* ResourceItemDefinition = UFortKismetLibrary::K2_GetResourceItemDefinition(BuildingSMActor->ResourceType);
	if (!ResourceItemDefinition) return Originals::OnDamageServer(_This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

	AFortPickup* Pickup = FortItemDefinition::SpawnPickup(ResourceItemDefinition, MaterialCount, 0, PlayerController->K2_GetActorLocation(), EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset, PlayerController->MyFortPawn);

	PlayerPawn->ServerHandlePickup(Pickup, 0.3f, FVector(), false);

	return Originals::OnDamageServer(_This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
}