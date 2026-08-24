#pragma once

#include "pch.h"

class GameModeBase
{
public:
	class Originals
	{
	public:
		inline static void (*HandleStartingNewPlayer)(AGameModeBase* _This, APlayerController* NewPlayer);
	};
public:
	static APawn* SpawnDefaultPawnFor(AGameModeBase* _This, AController* NewPlayer, AActor* StartSpot);

	static void HandleStartingNewPlayer(AGameModeBase* _This, APlayerController* NewPlayer);
};