#pragma once

#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"

class UHookingLibrary
{
public:
	static UHookingLibrary* Get()
	{
		static UHookingLibrary* Instance = new UHookingLibrary();
		return Instance;
	}
public:
	inline static void Patch(uintptr_t Target, uint32_t Byte)
	{
		DWORD Protect;
		VirtualProtect(LPVOID(Target), 4, PAGE_EXECUTE_READWRITE, &Protect);

		*(uintptr_t*)(Target) = Byte;

		DWORD Temp;
		VirtualProtect(LPVOID(Target), 4, Protect, &Temp);
	}

	inline static void Hook(uint64_t Address, PVOID Hook, void** Original = nullptr)
	{
		MH_CreateHook(PVOID(Address), Hook, Original);
		MH_EnableHook(PVOID(Address));
	}

	inline static void VTable(void* Index, uintptr_t Target, void* Vft, void** Original = nullptr)
	{
		void** VTable = *(void***)Index;

		if (Original) *Original = VTable[Target];

		DWORD Protect;
		VirtualProtect(&VTable[Target], sizeof(void*), PAGE_EXECUTE_READWRITE, &Protect);

		VTable[Target] = Vft;

		DWORD Temp;
		VirtualProtect(&VTable[Target], sizeof(void*), Protect, &Temp);
	}

	inline static void* _NpFH = nullptr;

	template <typename _Ot = void*>
	inline static void Exec(UFunction* _Function, void* _Hook, _Ot& _Original = _NpFH)
	{
		if (!_Function) return;
		if (!std::is_same_v<_Ot, void*>) _Original = (_Ot)_Function->ExecFunction;
		_Function->ExecFunction = reinterpret_cast<UFunction::FNativeFuncPtr>(_Hook);
	}

	template <typename _Ot = void*>
	inline static void Exec(const char* _Name, void* _Hook, _Ot& _Original = _NpFH)
	{
		UFunction* _Function = UObjectGlobals::StaticLoadObject<UFunction>(_Name);
		Exec(_Function, _Hook, _Original);
	}

	inline static uint8_t* AllocateNearbyPage(void* TargetAddress)
	{
		SYSTEM_INFO SystemInfo;

		GetSystemInfo(&SystemInfo);

		const uint64_t PageSize = SystemInfo.dwPageSize;
		const uint64_t StartAddress = (uint64_t(TargetAddress) & ~(PageSize - 1));
		const uint64_t MinAddress = min(StartAddress - 0x7FFFFF00, (uint64_t)SystemInfo.lpMinimumApplicationAddress);
		const uint64_t MaxAddress = max(StartAddress + 0x7FFFFF00, (uint64_t)SystemInfo.lpMaximumApplicationAddress);
		const uint64_t StartPage = (StartAddress - (StartAddress % PageSize));

		for (uint64_t PageOffset = 1; PageOffset; PageOffset++)
		{
			uint64_t ByteOffset = PageOffset * PageSize;
			uint64_t HighAddress = StartPage + ByteOffset;
			uint64_t LowAddress = (StartPage > ByteOffset) ? StartPage - ByteOffset : 0;

			bool NeedsExit = HighAddress > MaxAddress && LowAddress < MinAddress;

			if (HighAddress < MaxAddress) { if (void* OutAddress = VirtualAlloc((void*)HighAddress, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)) { return (uint8_t*)OutAddress; } }
			if (LowAddress > MinAddress) { if (void* OutAddress = VirtualAlloc((void*)LowAddress, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)) { return (uint8_t*)OutAddress; } }

			if (NeedsExit) { break; }
		}

		return nullptr;
	}

	inline static void Rel32(uintptr_t Target, void* Hook, void** Original = nullptr)
	{
		auto Impl = (uint8*)(Target);

		auto NearPage = AllocateNearbyPage(Impl);
		if (!NearPage) return;

		uint8_t ShellCode[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

		if (Hook != NULL)
		{
			memcpy(ShellCode + 6, &Hook, 8);
			memcpy(NearPage, ShellCode, sizeof(ShellCode));

			auto Offset = NearPage - (Impl + (int)5);

			memcpy(Impl + 1, &Offset, sizeof(int));
		}
		else { memset(Impl, 0x90, sizeof(int) + 1); }
	}
};

inline static void* (*ProcessEventOG)(UObject* Object, UFunction* Function, void* Parameters);
static void* ProcessEvent(UObject* Object, UFunction* Function, void* Parameters)
{
	if (Object && Function) {}
	return ProcessEventOG(Object, Function, Parameters);
}