#pragma once

#include "pch.h"

class NetDriver
{
public:
	class Originals
	{
	public:
		inline static void (*TickFlush)(UNetDriver* _This, float DeltaSeconds);
	};
public:
	static void TickFlush(UNetDriver* _This, float DeltaSeconds);
};