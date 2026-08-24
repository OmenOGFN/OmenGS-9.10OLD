#pragma once

#include "pch.h"

class FortPlayerController
{
public:
	class Originals
	{
	public:
		inline static void (*ServerAttemptAircraftJumpOG)(AFortPlayerController* _This, FRotator& ClientRotation);
	};
public:
	static void ServerExecuteInventoryItem(AFortPlayerController* _This, FGuid& ItemGuid);
};