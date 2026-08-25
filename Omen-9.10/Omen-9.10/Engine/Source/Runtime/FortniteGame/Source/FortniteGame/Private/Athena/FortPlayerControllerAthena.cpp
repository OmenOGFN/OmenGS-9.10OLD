#include "pch.h"
#include "../../Public/Athena/FortPlayerControllerAthena.h"

#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"

void FortPlayerControllerAthena::ServerPlayEmoteItem(AFortPlayerControllerAthena* _This, UFortMontageItemDefinitionBase* MontageItemDefinitionBase)
{
	AFortPlayerPawn* PlayerPawn = _This->MyFortPawn;
	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)_This->MyFortPawn;

	UClass* AbilityClass = nullptr;

	if (UAthenaDanceItemDefinition* DanceItemDefinition = (UAthenaDanceItemDefinition*)MontageItemDefinitionBase)
	{
		AbilityClass = DanceItemDefinition->CustomDanceAbility.Get() ? DanceItemDefinition->CustomDanceAbility.Get() : UObjectGlobals::StaticFindObject<UClass>(L"/Game/Abilities/Emotes/GAB_Emote_Generic.GAB_Emote_Generic_C");

		PlayerPawn->bMovingEmote = DanceItemDefinition->bMovingEmote;
		PlayerPawn->EmoteWalkSpeed = DanceItemDefinition->WalkForwardSpeed;
		PlayerPawn->bMovingEmoteForwardOnly = DanceItemDefinition->bMoveForwardOnly;
	}
	else if (UAthenaSprayItemDefinition* SprayItemDefinition = (UAthenaSprayItemDefinition*)MontageItemDefinitionBase) { AbilityClass = UObjectGlobals::StaticFindObject<UClass>(L"/Game/Abilities/Sprays/GAB_Spray_Generic.GAB_Spray_Generic_C"); }

	if (AbilityClass != nullptr)
	{
		FGameplayAbilitySpec Spec;
		Spec.ConstructAbilitySpec(Cast<UGameplayAbility>(AbilityClass->DefaultObject), 1, -1, MontageItemDefinitionBase);

		PlayerState->AbilitySystemComponent->GiveAbilityAndActivateOnce(&Spec.Handle, &Spec);
	}
}