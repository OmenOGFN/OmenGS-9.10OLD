#pragma once

#include "pch.h"

class FortPlayerController
{
public:
	class Originals
	{
	public:
		inline static void (*ServerAttemptAircraftJump)(AFortPlayerController* _This, FRotator& Rotation);
	};
public:
	static void ServerExecuteInventoryItem(AFortPlayerController* _This, FGuid& ItemGuid);
	static void ServerAttemptAircraftJump(AFortPlayerController* _This, FRotator& Rotation);
};