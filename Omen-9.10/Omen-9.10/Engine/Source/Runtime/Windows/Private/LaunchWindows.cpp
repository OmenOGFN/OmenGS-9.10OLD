#include "pch.h"
#include "Config.h"

#include "Engine/Plugins/HookingLibrary/HookingLibrary.h"
#include "Engine/Plugins/OnlineGameplayFramework/Source/McpProfileSys/Public/McpProfileGroup.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameEngine.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Athena/FortGameModeAthena.h"

#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Building/BuildingActor.h"
#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Building/BuildingFoundation.h"

#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Items/FortInventory.h"
#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Items/FortWorldItem.h"

#include "Engine/Source/Runtime/FortniteGame/Source/FortniteGame/Public/Player/FortPlayerController.h"

void Main()
{
    AllocConsole();

    FILE* File = nullptr;

    freopen_s(&File, "CONOUT$", "w+", stdout);
    freopen_s(&File, "CONIN$", "r", stdin);
    freopen_s(&File, "CONOUT$", "w+", stderr);

    SetConsoleTitleA("Omen: 9.10: Setting Up");

    LOG(std::format("ImageBase: 0x{:X}", InSDKUtils::GetImageBase()));

    if (MH_Initialize() != MH_OK) { ExitThread(0); }

    Sleep(2500);

    SetConsoleTitleA("Omen: 9.10: Patching");
    *reinterpret_cast<bool*>(InSDKUtils::GetImageBase() + 0x5BE45F3) = false; // GIsClient
    *reinterpret_cast<bool*>(InSDKUtils::GetImageBase() + 0x5BE45F4) = true; // GIsServer

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x1FF9930, ProcessEvent, (void**)&ProcessEventOG);

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0xB10DE0, McpProfileGroup::SendRequestNow, (void**)&McpProfileGroup::Originals::SendRequestNow);

    UHookingLibrary::Get()->VTable(UAbilitySystemComponent::GetDefaultObj(), 0xF4, AbilitySystemComponent::InternalServerTryActivateAbility);
    UHookingLibrary::Get()->VTable(UFortAbilitySystemComponent::GetDefaultObj(), 0xF4, AbilitySystemComponent::InternalServerTryActivateAbility);
    UHookingLibrary::Get()->VTable(UFortAbilitySystemComponentAthena::GetDefaultObj(), 0xF4, AbilitySystemComponent::InternalServerTryActivateAbility);

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x315D2C0, GameEngine::GetMaxTickRate);

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x2ED22B0, NetDriver::TickFlush, (void**)&NetDriver::Originals::TickFlush);

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x31AC180, World::GetNetMode);
    UHookingLibrary::Get()->Rel32(InSDKUtils::GetImageBase() + 0x3164856, World::ListenCall);

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x1098E10, GameModeBase::SpawnDefaultPawnFor);
    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x13F0B70, GameModeBase::HandleStartingNewPlayer, (void**)&GameModeBase::Originals::HandleStartingNewPlayer);

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x2D9AE90, GameSession::KickPlayer, (void**)&GameSession::Originals::KickPlayer);

    UHookingLibrary::Get()->VTable(AFortPlayerControllerAthena::GetDefaultObj(), 0x840 / 8, PlayerController::ServerAcknowledgePossession, (void**)&PlayerController::Originals::ServerAcknowledgePossession); // 0x108

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x10915D0, FortGameModeAthena::ReadyToStartMatch, (void**)&FortGameModeAthena::Originals::ReadyToStartMatch);

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x1A49CB0, BuildingActor::OnDamageServer, (void**)&BuildingActor::Originals::OnDamageServer);

    UHookingLibrary::Get()->Exec("/Script/FortniteGame.BuildingFoundation.SetDynamicFoundationEnabled", BuildingFoundation::SetDynamicFoundationEnabled);
    UHookingLibrary::Get()->Exec("/Script/FortniteGame.BuildingFoundation.SetDynamicFoundationTransform", BuildingFoundation::SetDynamicFoundationTransform);

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x1837A60, FortInventory::RemoveInventoryItem);

    UHookingLibrary::Get()->VTable(UFortWorldItem::GetDefaultObj(), 0x94, FortWorldItem::SetLoadedAmmo);

    UHookingLibrary::Get()->VTable(AFortPlayerControllerAthena::GetDefaultObj(), 0xFE8 / 8, FortPlayerController::ServerExecuteInventoryItem);

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x29A2100, CoreGlobals::ReturnNull);
    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x1403040, CoreGlobals::ReturnNull);
    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x1F84D30, CoreGlobals::ReturnNull);

    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x890540, CoreGlobals::ReturnTrue); // CanActivateAbility
    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x1FAA590, CoreGlobals::ReturnTrue);

    //UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x1F84B7A, CoreGlobals::ReturnFalse); // CollectGarbage
    UHookingLibrary::Get()->Hook(InSDKUtils::GetImageBase() + 0x14A6650, CoreGlobals::ReturnFalse); // NoMCP

    //UHookingLibrary::Get()->Patch(InSDKUtils::GetImageBase() + 0x10D9CCC, 0x85); // GameSessionPatch
    //UHookingLibrary::Get()->Patch(InSDKUtils::GetImageBase() + 0x31AEBCA, 0x74); // EncryptionPatch

    Sleep(1000);

    SetConsoleTitleA("Omen: 9.10: Loading World");
    UWorld::GetWorld()->OwningGameInstance->LocalPlayers.Remove(0);
    UGameplayStatics::OpenLevel(UWorld::GetWorld(), UKismetStringLibrary::Conv_StringToName(L"Athena_Terrain"), false, FString());
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ReasonForCall, LPVOID lpReserved)
{
    switch (ReasonForCall)
    {
    case DLL_PROCESS_ATTACH:
        std::thread(Main).detach();
        break;
    default:
        break;
    }
    return TRUE;
}