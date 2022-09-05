#ifndef RX_CURL2_HPP_
#define RX_CURL2_HPP_

#include "types.hpp"

#include <rxcpp/rx.hpp>
#include <curl/curl.h>

#include <string>
#include <future>
#include <map>

struct HttpRequest
{
	std::string url;
	HttpRequestMethod method;
	std::map<std::string, std::string> headers;
	std::string body;
};

struct HttpResponse
{
	const HttpRequest request;
	std::string response_body;
    std::string error;
    CURLcode code;
	int http_status;
};

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	curl_off_t nread = strlen((const char *)stream);
	memcpy(ptr, stream, nread);
	return nread;
}

struct RxCurl2
{
    rxcpp::observable<HttpResponse> create(HttpRequest request)
    {
		
		
		return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> &out)
		{
			std::future<HttpResponse> result
			{
				std::async(std::launch::async,
				[&]()-> HttpResponse
				{
					CURL *curl;
					CURLcode res;
					HttpResponse response{};

					curl = curl_easy_init();
					if(curl)
					{
						curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());
						curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.response_body);
						curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

						switch(request.method)
						{
							case HttpRequestMethod::kGet:
							{
								// For future use.
							}
							break;
							case HttpRequestMethod::kPost:
							{
								curl_easy_setopt(curl, CURLOPT_POST, 1L);
								curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());
							}
							break;
							case HttpRequestMethod::kPatch:
							{
								curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST,"PATCH");
								curl_easy_setopt(curl, CURLOPT_POSTFIELDS,request.body.c_str());
							}
							break;
							case HttpRequestMethod::kPut:
							{
								curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
								curl_easy_setopt(curl, CURLOPT_READDATA, request.body.c_str());
								curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
								curl_easy_setopt(curl, CURLOPT_INFILESIZE, strlen(request.body.c_str()));
							}
							break;
							case HttpRequestMethod::kDelete:
							{
								curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST,"DELETE");
								curl_easy_setopt(curl, CURLOPT_POSTFIELDS,request.body.c_str());
							}
							break;
						}

						struct curl_slist *headers;
						std::vector<std::string> strings;
						for (auto& h : request.headers)
						{
							strings.push_back(h.first + ": " + h.second);
							headers = curl_slist_append(headers, strings.back().c_str());
						}
						if (!!headers)
						{
							curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
						}

						curl_easy_setopt(curl, CURLOPT_USERAGENT, "PostmanRuntime/7.29.0");
						curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
						curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
						curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

						response.code = curl_easy_perform(curl);
						response.error = curl_easy_strerror(res);
						curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.http_status);

						curl_easy_cleanup(curl);
					}
					return response;
				})
			};

			auto res = result.get();
			out.on_next(res);
			out.on_completed();
		});
    }
};

RxCurl2* create_rxcurl2()
{
	auto *r = new RxCurl2{};
	return r;
};

#endif