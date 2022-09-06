#ifndef REACTIVE_CURL_HPP
#define REACTIVE_CURL_HPP

#include <curl/curl.h>

#include <string>
#include <map>


#include <algorithm>
#include <atomic>
#include <deque>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <assert.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <fcntl.h>
#include <string.h>



enum class RequestType { GET, PUT, POST, HEAD, DELETE, PATCH, OPTIONS, INVALID };

using completion_fn_t = std::function<void (const Result& result)>;

using HeaderFileds = std::map<std::string, std::string>;

enum class HttpRequestMethod
{
	kGet,
	kPost,
	kPatch,
	kPut,
	kDelete,
};

struct HttpRequest
{
	std::string url;
	HttpRequestMethod method;
	std::map<std::string, std::string> headers;
	std::string body;
};

struct HttpResponse
{
	std::string body;
	HeaderFileds headers;
    std::string error;
    int curl_code;
	int http_code;
};

#endif //REACTIVE_CURL_HPP