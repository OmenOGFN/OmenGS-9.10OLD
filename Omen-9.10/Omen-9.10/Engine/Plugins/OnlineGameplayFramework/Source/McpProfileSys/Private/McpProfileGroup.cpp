#include "pch.h"
#include "../Public/McpProfileGroup.h"

void McpProfileGroup::SendRequestNow(UMcpProfileGroup* _This, const void* HttpRequest, FBaseUrlContext::EContextCredentials ContextCredentials)
{
	Originals::SendRequestNow(_This, HttpRequest, FBaseUrlContext::CXC_Public);
}