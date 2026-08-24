#include "pch.h"
#include "../Classes/Engine/World.h"

ENetMode World::GetNetMode(UWorld* _This) { return ENetMode::DedicatedServer; }

bool World::ListenCall(UWorld* _This)
{
	FName NetDriverDefinition = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");
	UNetDriver* NetDriver = UEngine::GetEngine()->CreateNetDriver(UWorld::GetWorld(), NetDriverDefinition);

	NetDriver->World = UWorld::GetWorld();
	NetDriver->NetDriverName = NetDriverDefinition;

	FString Error;
	FURL URL;
	URL.Port = 7777;

	if (!NetDriver->InitListen(UWorld::GetWorld(), URL, false, Error)) return false;

	NetDriver->SetWorld(UWorld::GetWorld());

	UWorld::GetWorld()->NetDriver = NetDriver;

	for (FLevelCollection& LevelCollection : UWorld::GetWorld()->LevelCollections) { LevelCollection.NetDriver = UWorld::GetWorld()->NetDriver; }

	return true;
}