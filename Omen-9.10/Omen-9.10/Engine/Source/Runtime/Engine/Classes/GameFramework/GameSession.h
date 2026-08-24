#pragma once

#include "pch.h"

class GameSession
{
public:
	class Originals
	{
	public:
		inline static void (*KickPlayer)(AGameSession* _This, AController* Controller);
	};
public:
	static void KickPlayer(AGameSession* _This, AController* Controller);
};