#ifndef REACTIVE_CURL_HPP
#define REACTIVE_CURL_HPP
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

#endif //REACTIVE_CURL_HPP