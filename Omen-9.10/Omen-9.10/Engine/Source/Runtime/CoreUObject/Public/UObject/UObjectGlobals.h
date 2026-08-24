#pragma once

#include "pch.h"

inline UObject* (*StaticFindObjectInternal)(UClass*, UObject* Package, const wchar_t* OrigInName, bool ExactClass) = decltype(StaticFindObjectInternal)(InSDKUtils::GetImageBase() + 0x2001E30);
inline UObject* (*StaticLoadObjectInternal)(UClass* Class, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, uint32_t LoadFlags, UObject* Sandbox, bool bAllowObjectReconciliation, void*) = decltype(StaticLoadObjectInternal)(InSDKUtils::GetImageBase() + 0x2003110);

class UObjectGlobals
{
public:
	template <typename T>
	inline static T* StaticFindObject(std::wstring ObjectName) { return (T*)StaticFindObjectInternal(T::StaticClass(), nullptr, ObjectName.c_str(), false); }

	template <typename T = UObject>
	inline static T* StaticLoadObject(const std::string& ObjectName)
	{
		auto Name = std::wstring(ObjectName.begin(), ObjectName.end());

		T* FindObject = StaticFindObject<T>(Name);
		if (!FindObject) { FindObject = (T*)StaticLoadObjectInternal(T::StaticClass(), nullptr, Name.c_str(), nullptr, 0, nullptr, false, nullptr); }
		return FindObject;
	}
};