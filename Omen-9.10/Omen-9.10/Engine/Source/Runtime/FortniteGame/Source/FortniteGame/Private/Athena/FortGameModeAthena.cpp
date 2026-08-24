#include "pch.h"
#include "../../Public/Athena/FortGameModeAthena.h"
#include "../../Public/Building/BuildingFoundation.h"

#include "Config.h"

#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"

bool FortGameModeAthena::ReadyToStartMatch(AFortGameModeAthena* _This)
{
	if (!_This || !_This->GameState) return false;

	AFortGameStateAthena* GameState = (AFortGameStateAthena*)_This->GameState;

	float CurrentTime = UGameplayStatics::GetDefaultObj()->GetTimeSeconds(UWorld::GetWorld());
	float WarmupTime = 30.f;

	static bool Init = false;
	if (!Init)
	{
		Init = true;

		UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("FortPlaylistAthena Playlist_DefaultSolo.Playlist_DefaultSolo");
		if (!Playlist) { UE_LOG(LogGameMode, Error, "Failed To Find Playlist."); return false; }

		if (UConfig->bIsCustom1v1Map)
		{
			Playlist->RespawnType = EAthenaRespawnType::InfiniteRespawn;
			Playlist->RespawnTime = FScalableFloat(2.f);
		}

		GameState->CurrentPlaylistInfo.BasePlaylist = Playlist;
		GameState->CurrentPlaylistInfo.OverridePlaylist = Playlist;
		GameState->CurrentPlaylistInfo.PlaylistReplicationKey++;
		GameState->CurrentPlaylistInfo.MarkArrayDirty();
		GameState->OnRep_CurrentPlaylistInfo();

		_This->CurrentPlaylistName = Playlist->PlaylistName;
		_This->WarmupRequiredPlayerCount = 1;

		_This->GameSession->MaxPlayers = Playlist->MaxPlayers;
		_This->GameSession->MaxSpectators = 0;
		_This->GameSession->MaxPartySize = Playlist->MaxSquadSize;
		_This->GameSession->bRequiresPushToTalk = false;
		_This->GameSession->SessionName = UKismetStringLibrary::Conv_StringToName(FString(L"GameSession"));

		GameState->WarmupCountdownEndTime = CurrentTime + WarmupTime;
		_This->WarmupCountdownDuration = WarmupTime;
		GameState->WarmupCountdownStartTime = CurrentTime;
		_This->WarmupEarlyCountdownDuration = WarmupTime;

		GameState->CurrentPlaylistId = Playlist->PlaylistId;
		GameState->OnRep_CurrentPlaylistId();

		GameState->CachedSafeZoneStartUp = Playlist->SafeZoneStartUp;
		GameState->AirCraftBehavior = Playlist->AirCraftBehavior;
		GameState->OnRep_Aircraft();
	}

	static bool Initialized = false;
	if (!Initialized)
	{
		Initialized = true;

		GameState->DefaultParachuteDeployTraceForGroundDistance = UConfig->bIsLateGame ? 700 : 7000;

		for (auto& Level : GameState->CurrentPlaylistInfo.BasePlaylist->AdditionalLevels)
		{
			bool Success = false;
			ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(UWorld::GetWorld(), Level, FVector(), FRotator(), &Success);
			if (Success) GameState->AdditionalPlaylistLevelsStreamed.Add(Level.ObjectID.AssetPathName);
		}

		GameState->OnRep_AdditionalPlaylistLevelsStreamed();
		GameState->OnFinishedStreamingAdditionalPlaylistLevel();

		BuildingFoundation::ShowFoundation(UObjectGlobals::StaticLoadObject<ABuildingFoundation>("/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Athena_POI_50x53_Volcano"));
		BuildingFoundation::ShowFoundation(UObjectGlobals::StaticLoadObject<ABuildingFoundation>("/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.SLAB_2"));

		_This->bWorldIsReady = true;

		SetConsoleTitleA("Omen: 9.10: Listening");
	}

	if (GameState->PlayersLeft > 0) { return true; }
	else
	{
		GameState->WarmupCountdownEndTime = CurrentTime + WarmupTime;
		_This->WarmupCountdownDuration = WarmupTime;
		GameState->WarmupCountdownStartTime = CurrentTime;
		_This->WarmupEarlyCountdownDuration = WarmupTime;
	}

	return false;
}