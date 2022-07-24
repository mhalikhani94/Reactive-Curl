#pragma once
#include <map>
#include <string>
#include <vector>
#include <rxcpp/rx-observable.hpp>
#include "types.hpp"

struct RxCurl;
struct HttpResponse;

class SingletonRx
{
public:
	static SingletonRx& instance();

	SingletonRx(SingletonRx&& i) = delete;
	SingletonRx(const SingletonRx& i) = delete;
	SingletonRx& operator=(SingletonRx&& i) = delete;
	SingletonRx operator=(const SingletonRx& i) = delete;

	~SingletonRx() = default;

	void send_request(const std::string& url, std::string method, std::map<std::string, std::string> headers,
	                  const std::string& body, std::promise<Response>* pr = {});

	void set_curl_config(long timeout = 500) const;


private:
	SingletonRx();
	std::vector<rxcpp::observable<HttpResponse>> m_requests;
	RxCurl* m_rx_curl;

	rxcpp::observable<std::string> response_message;
	rxcpp::observable<int> status_codes;
	std::string html;
};
