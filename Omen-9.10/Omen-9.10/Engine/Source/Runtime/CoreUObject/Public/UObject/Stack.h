#pragma once

#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"

class FFrame : public FOutputDevice
{
public:
    UFunction* Node;
    UObject* Object;

    uint8* Code;
    uint8* Locals;

    void* MostRecentProperty;

    uint8_t* MostRecentPropertyAddress;
    uint8_t _Padding1[0x40];

    const UField* PropertyChainForCompiledIn;
public:
    inline void StepCompiledIn(void* const Result = nullptr)
    {
        if (Code)
        {
            static void (*StepCompiledInCode)(FFrame*, UObject*, void* const) = decltype(StepCompiledInCode)(InSDKUtils::GetImageBase() + 0x20037A0);
            StepCompiledInCode(this, Object, Result);
        }
        else
        {
            const UField* _Prop = *(const UField**)(__int64(this) + 0x80);
            if (_Prop)
            {
                *(const UField**)(__int64(this) + 0x80) = *(const UField**)(__int64(_Prop) + 0x28);
                static void (*StepExplicitProperty)(FFrame*, void* const, const UField*) = decltype(StepExplicitProperty)(InSDKUtils::GetImageBase() + 0x20037D0);
                StepExplicitProperty(this, Result, _Prop);
            }
        }
    }

    __forceinline void IncrementCode() { Code += !!Code; }
};