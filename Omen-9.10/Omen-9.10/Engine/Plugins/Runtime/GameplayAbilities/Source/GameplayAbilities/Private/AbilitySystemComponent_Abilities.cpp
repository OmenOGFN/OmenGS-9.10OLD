#include "pch.h"
#include "../Public/AbilitySystemComponent.h"

void AbilitySystemComponent::InternalServerTryActivateAbility(UAbilitySystemComponent* _This, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey, const FGameplayEventData* TriggerEventData)
{
	FGameplayAbilitySpec* Spec = _This->FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		_This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	const UGameplayAbility* AbilityToActivate = Spec->Ability;
	if (!AbilityToActivate)
	{
		_This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	UGameplayAbility* InstanceAbility = nullptr;
	Spec->InputPressed = true;

	if (_This->InternalTryActivateAbility(Handle, PredictionKey, &InstanceAbility, nullptr, TriggerEventData)) {}
	else
	{
		_This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		Spec->InputPressed = false;
		_This->ActivatableAbilities.MarkItemDirty(*Spec);
	}
}