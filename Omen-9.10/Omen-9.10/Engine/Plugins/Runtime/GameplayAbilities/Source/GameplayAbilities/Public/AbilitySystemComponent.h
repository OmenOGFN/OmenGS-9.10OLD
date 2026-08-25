#pragma once

#include "pch.h"

inline void (*GiveAbilityInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec) = decltype(GiveAbilityInternal)(InSDKUtils::GetImageBase() + 0x8A3F30);

class AbilitySystemComponent
{
public:
	static void InternalServerTryActivateAbility(UAbilitySystemComponent* _This, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey, const FGameplayEventData* TriggerEventData);
};