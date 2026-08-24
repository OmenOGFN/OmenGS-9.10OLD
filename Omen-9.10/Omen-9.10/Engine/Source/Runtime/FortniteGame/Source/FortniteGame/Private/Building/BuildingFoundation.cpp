#include "pch.h"
#include "../../Public/Building/BuildingFoundation.h"

void BuildingFoundation::ShowFoundation(ABuildingFoundation* _This)
{
	if (!_This) return;

	_This->DynamicFoundationType = EDynamicFoundationType::Static;
	_This->FoundationEnabledState = EDynamicFoundationEnabledState::Enabled;
	_This->SetDynamicFoundationEnabled(true);
	_This->bServerStreamedInLevel = true;
	_This->OnRep_ServerStreamedInLevel();
}

void BuildingFoundation::SetDynamicFoundationEnabled(ABuildingFoundation* _This, FFrame& Stack)
{
	bool Enabled;

	Stack.StepCompiledIn(&Enabled);
	Stack.IncrementCode();

	EDynamicFoundationEnabledState DynamicFoundationEnabledState = Enabled ? EDynamicFoundationEnabledState::Enabled : EDynamicFoundationEnabledState::Disabled;

	_This->FoundationEnabledState = DynamicFoundationEnabledState;
}

void BuildingFoundation::SetDynamicFoundationTransform(ABuildingFoundation* _This, FFrame& Stack)
{
	FTransform Transform;

	Stack.StepCompiledIn(&Transform);
	Stack.IncrementCode();

	_This->DynamicFoundationTransform = Transform;
	_This->StreamingData.FoundationLocation = Transform.Translation;
	_This->StreamingData.BoundingBox = _This->StreamingBoundingBox;
}