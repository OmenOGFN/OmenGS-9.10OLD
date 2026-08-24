#pragma once

#include "pch.h"

template <typename T>
T* Cast(UObject* Object)
{
	if (!Object || !Object->IsA(T::StaticClass())) return nullptr;
	return (T*)Object;
}