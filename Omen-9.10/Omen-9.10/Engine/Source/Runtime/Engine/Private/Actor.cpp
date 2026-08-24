#include "pch.h"
#include "../Classes/Engine/Actor.h"

ENetMode Actor::GetNetMode(AActor* _This) { return ENetMode::DedicatedServer; }