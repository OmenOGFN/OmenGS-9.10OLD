#include "pch.h"
#include "../Classes/GameFramework/PlayerController.h"

#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectBaseUtility.h"

#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Items/FortInventory.h"

void PlayerController::ServerAcknowledgePossession(APlayerController* _This, APawn* Pawn)
{
	_This->AcknowledgedPawn = Pawn;

	AFortPlayerControllerAthena* PlayerController = (AFortPlayerControllerAthena*)_This;
	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PlayerController->PlayerState;

	PlayerState->ApplyCharacterCustomization(PlayerController->MyFortPawn);

	UFortGlobals::InitializePlayerGameplayAbilities(UObjectBaseUtility::GetInterfaceAddress<IAbilitySystemInterface>(PlayerState));

	if (AFortGameModeAthena* GameMode = Cast<AFortGameModeAthena>(UWorld::GetWorld()->AuthorityGameMode))
	{
		for (const FItemAndCount& StartingItem : GameMode->StartingItems) { if (StartingItem.Count > 0) { FortInventory::CreateWorldItem(PlayerController->WorldInventory, StartingItem.Item, StartingItem.Count); } }

		FortInventory::CreateWorldItem(PlayerController->WorldInventory, PlayerController->CosmeticLoadoutPC.Pickaxe->WeaponDefinition);
	}

	return Originals::ServerAcknowledgePossession(_This, Pawn);
}