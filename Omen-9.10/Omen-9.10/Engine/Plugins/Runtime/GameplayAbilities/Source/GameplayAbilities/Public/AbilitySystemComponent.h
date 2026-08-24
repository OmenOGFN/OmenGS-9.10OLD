#pragma once

#include "pch.h"

inline void (*ConstructAbilitySpecInternal)(FGameplayAbilitySpec*, UGameplayAbility*, int, int, UObject*) = decltype(ConstructAbilitySpecInternal)(InSDKUtils::GetImageBase() + 0x8C8410);
inline void (*GiveAbilityInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec) = decltype(GiveAbilityInternal)(InSDKUtils::GetImageBase() + 0x8A3F30);

class AbilitySystemComponent
{
public:
	static void InternalServerTryActivateAbility(UAbilitySystemComponent* _This, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey, const FGameplayEventData* TriggerEventData);
};