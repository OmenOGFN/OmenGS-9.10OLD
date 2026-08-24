#include "pch.h"
#include "../Classes/Engine/NetDriver.h"

void NetDriver::TickFlush(UNetDriver* _This, float DeltaSeconds)
{
	if (_This->ReplicationDriver != nullptr) { _This->ReplicationDriver->ServerReplicateActors(DeltaSeconds); }

	return Originals::TickFlush(_This, DeltaSeconds);
}