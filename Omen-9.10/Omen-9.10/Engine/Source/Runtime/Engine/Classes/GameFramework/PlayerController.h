#pragma once

#include "pch.h"

class PlayerController
{
public:
	class Originals
	{
	public:
		inline static void (*ServerAcknowledgePossession)(APlayerController* _This, APawn* Pawn);
	};
public:
	static void ServerAcknowledgePossession(APlayerController* _This, APawn* Pawn);
};