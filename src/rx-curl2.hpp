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

size_t write_to_string(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

struct RxCurl2
{
    rxcpp::observable<HttpResponse> create(HttpRequest request)
    {
		return rxcpp::observable<>::create<HttpResponse>([=](rxcpp::subscriber<HttpResponse> &out)
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
						curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);

						if (request.method == HttpRequestMethod::kPost)
						{
							curl_easy_setopt(curl, CURLOPT_POST, 1L);
							curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());
						}

						struct curl_slist *header_list = NULL;
						std::string headerString;

						for (auto header = request.headers.begin(); header != request.headers.end(); ++header)
						{ 	
        					headerString = header->first;
        					headerString.append(": ").append(header->second);
       						header_list = curl_slist_append(header_list, headerString.c_str()); 
    					}
						curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

						curl_easy_setopt(curl, CURLOPT_USERAGENT, "PostmanRuntime/7.29.0");
						curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
						curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
						curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

						res = curl_easy_perform(curl);
						curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.http_status);

						response.code = res;
						response.error = curl_easy_strerror(res);

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