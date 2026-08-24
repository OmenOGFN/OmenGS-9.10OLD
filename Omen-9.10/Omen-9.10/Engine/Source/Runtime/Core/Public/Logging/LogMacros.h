#pragma once

#include "pch.h"

inline void LOG(const std::string& Message)
{
	std::cout << "LogOmen >> " << Message << std::endl;
}

enum class ELogLevel
{
	Fatal,
	Error,
	Warning,
	Verbose,
	VeryVerbose,
	Log
};

inline constexpr ELogLevel Fatal = ELogLevel::Fatal;
inline constexpr ELogLevel Error = ELogLevel::Error;
inline constexpr ELogLevel Warning = ELogLevel::Warning;
inline constexpr ELogLevel Verbose = ELogLevel::Verbose;
inline constexpr ELogLevel VeryVerbose = ELogLevel::VeryVerbose;
inline constexpr ELogLevel Log = ELogLevel::Log;

inline const char* ToString(ELogLevel LogLevel)
{
	switch (LogLevel)
	{
	case ELogLevel::Fatal: return "Fatal";
	case ELogLevel::Error: return "Error";
	case ELogLevel::Warning: return "Warning";
	case ELogLevel::Verbose: return "Verbose";
	case ELogLevel::VeryVerbose: return "VeryVerbose";
	case ELogLevel::Log: return "Log";
	default: return "Unknown";
	}
}

inline const wchar_t* ToWString(ELogLevel LogLevel)
{
	switch (LogLevel)
	{
	case ELogLevel::Fatal: return L"Fatal";
	case ELogLevel::Error: return L"Error";
	case ELogLevel::Warning: return L"Warning";
	case ELogLevel::Verbose: return L"Verbose";
	case ELogLevel::VeryVerbose: return L"VeryVerbose";
	case ELogLevel::Log: return L"Log";
	default: return L"Unknown";
	}
}

inline void SetConsoleColorByLogLevel(ELogLevel LogLevel)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	switch (LogLevel)
	{
	case ELogLevel::Fatal:
		Color = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case ELogLevel::Error:
		Color = FOREGROUND_RED;
		break;
	case ELogLevel::Warning:
		Color = FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case ELogLevel::Verbose:
		Color = FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	case ELogLevel::VeryVerbose:
		Color = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;
	case ELogLevel::Log:
	default:
		Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	}

	SetConsoleTextAttribute(hConsole, Color);
}

inline void ResetConsoleColor() { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); }

struct FLogCategory
{
	const char* Name;

	ELogLevel RuntimeVerbosity;
};

#if _DEBUG
#define DEFINE_LOG_CATEGORY(CategoryName)                                      \
        inline FLogCategory CategoryName = { #CategoryName, ELogLevel::VeryVerbose }
#else
#define DEFINE_LOG_CATEGORY(CategoryName)                                      \
        inline FLogCategory CategoryName = { #CategoryName, ELogLevel::Log }
#endif

inline void UE_LOGImpl(FLogCategory& LogCategory, ELogLevel LogLevel, const char* Message)
{
	if ((int)LogLevel > (int)LogCategory.RuntimeVerbosity) { return; }

	SetConsoleColorByLogLevel(LogLevel);
	std::cout << LogCategory.Name << ": " << ToString(LogLevel) << ": " << Message << std::endl;
	ResetConsoleColor();
}

inline std::string WStringToUTF8(const std::wstring& wstr) { return std::string(wstr.begin(), wstr.end()); }

inline void UE_LOGWImpl(FLogCategory& LogCategory, ELogLevel LogLevel, const wchar_t* Message)
{
	if ((int)LogLevel > (int)LogCategory.RuntimeVerbosity) { return; }

	std::wstring WFinal(Message);
	std::string FinalBuffer = WStringToUTF8(WFinal);

	SetConsoleColorByLogLevel(LogLevel);
	std::cout << LogCategory.Name << ": " << ToString(LogLevel) << ": " << FinalBuffer << std::endl;
	ResetConsoleColor();
}

inline void UE_LOG_DISPATCH(FLogCategory& LogCategory, ELogLevel LogLevel, const char* Format, ...)
{
	va_list Args;
	va_start(Args, Format);

	char Buffer[2048];
	vsnprintf(Buffer, sizeof(Buffer), Format, Args);
	va_end(Args);

	UE_LOGImpl(LogCategory, LogLevel, Buffer);
}

inline void UE_LOG_WDISPATCH(FLogCategory& LogCategory, ELogLevel LogLevel, const wchar_t* Format, ...)
{
	va_list Args;
	va_start(Args, Format);

	wchar_t Buffer[2048];
	vswprintf(Buffer, sizeof(Buffer) / sizeof(wchar_t), Format, Args);
	va_end(Args);

	UE_LOGWImpl(LogCategory, LogLevel, Buffer);
}

#define UE_LOG(Category, Verbosity, Format, ...) \
UE_LOG_DISPATCH(Category, Verbosity, Format, ##__VA_ARGS__)

#define UE_LOG_W(Category, Verbosity, Format, ...) \
UE_LOG_WDISPATCH(Category, Verbosity, Format, ##__VA_ARGS__)

DEFINE_LOG_CATEGORY(LogLaunch);
DEFINE_LOG_CATEGORY(LogServer);
DEFINE_LOG_CATEGORY(LogAthena);
DEFINE_LOG_CATEGORY(LogMatch);
DEFINE_LOG_CATEGORY(LogTeams);
DEFINE_LOG_CATEGORY(LogStorm);
DEFINE_LOG_CATEGORY(LogInventory);
DEFINE_LOG_CATEGORY(LogWeapons);
DEFINE_LOG_CATEGORY(LogAbilities);
DEFINE_LOG_CATEGORY(LogItems);
DEFINE_LOG_CATEGORY(LogVehicles);
DEFINE_LOG_CATEGORY(LogBuilding);
DEFINE_LOG_CATEGORY(LogAI);
DEFINE_LOG_CATEGORY(LogPlayer);
DEFINE_LOG_CATEGORY(LogPawn);
DEFINE_LOG_CATEGORY(LogCreative);
DEFINE_LOG_CATEGORY(LogQuests);
DEFINE_LOG_CATEGORY(LogTemp);
DEFINE_LOG_CATEGORY(LogCheats);
DEFINE_LOG_CATEGORY(LogKismet);
DEFINE_LOG_CATEGORY(LogGameMode);
DEFINE_LOG_CATEGORY(LogGameState);
DEFINE_LOG_CATEGORY(LogGameSessions);
DEFINE_LOG_CATEGORY(LogNet);