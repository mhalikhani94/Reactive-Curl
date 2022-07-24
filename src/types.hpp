#pragma once
#include <string>

enum class HttpRequestType
{
	kGet,
	kPost,
};


struct Response
{
	int http_status;
	std::string http_response;
};