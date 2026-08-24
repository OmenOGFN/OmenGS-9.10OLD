#include "pch.h"
#include "../Classes/GameFramework/GameModeBase.h"

#include "Engine/Source/Runtime/CoreUObject/Public/Templates/Casts.h"

APawn* GameModeBase::SpawnDefaultPawnFor(AGameModeBase* _This, AController* NewPlayer, AActor* StartSpot)
{
	return _This->SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetTransform());
}

void GameModeBase::HandleStartingNewPlayer(AGameModeBase* _This, APlayerController* NewPlayer)
{
	Originals::HandleStartingNewPlayer(_This, NewPlayer);

	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)NewPlayer->PlayerState;

	PlayerState->SquadId = PlayerState->TeamIndex - 3;
	PlayerState->OnRep_SquadId();

	PlayerState->WorldPlayerId = PlayerState->PlayerID;

	AFortGameStateAthena* GameState = (AFortGameStateAthena*)_This->GameState;

	FGameMemberInfo GameMemberInfo;
	GameMemberInfo.ReplicationID = -1;
	GameMemberInfo.ReplicationKey = -1;
	GameMemberInfo.MostRecentArrayReplicationKey = -1;
	GameMemberInfo.SquadId = PlayerState->SquadId;
	GameMemberInfo.TeamIndex = PlayerState->TeamIndex;
	GameMemberInfo.MemberUniqueId = PlayerState->UniqueId;

	GameState->GameMemberInfoArray.Members.Add(GameMemberInfo);
	GameState->GameMemberInfoArray.MarkArrayDirty();

	GameMemberInfo.PostReplicatedAdd(&GameState->GameMemberInfoArray);

	if (AFortPlayerControllerAthena* PlayerController = (AFortPlayerControllerAthena*)NewPlayer) { if (!PlayerController->MatchReport) PlayerController->MatchReport = Cast<UAthenaPlayerMatchReport>(UGameplayStatics::SpawnObject(UAthenaPlayerMatchReport::StaticClass(), PlayerController)); }
}