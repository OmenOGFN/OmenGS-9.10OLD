#pragma once

#include "pch.h"

class McpProfileGroup
{
public:
	class Originals
	{
	public:
		inline static void (*SendRequestNow)(UMcpProfileGroup* _This, const void* HttpRequest, FBaseUrlContext::EContextCredentials ContextCredentials);
	};
public:
	static void SendRequestNow(UMcpProfileGroup* _This, const void* HttpRequest, FBaseUrlContext::EContextCredentials ContextCredentials);
};