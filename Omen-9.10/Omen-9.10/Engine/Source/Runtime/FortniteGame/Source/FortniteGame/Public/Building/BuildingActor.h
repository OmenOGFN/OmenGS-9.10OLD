#pragma once

#include "pch.h"

class BuildingActor
{
public:
	class Originals
	{
	public:
		inline static void (*OnDamageServer)(ABuildingActor* _This, float Damage, FGameplayTagContainer& DamageTags, FVector& Momentum, FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle& EffectContext);
	};
public:
	static void OnDamageServer(ABuildingActor* _This, float Damage, FGameplayTagContainer& DamageTags, FVector& Momentum, FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle& EffectContext);
};