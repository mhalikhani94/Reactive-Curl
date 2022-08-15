#pragma once
#include <map>
#include <string>
#include <vector>
#include <rxcpp/rx-observable.hpp>
#include "types.hpp"

struct RxCurl;
struct HttpResponse;

class RxRequestManager
{
public:
	static RxRequestManager& instance();

	RxRequestManager(RxRequestManager&& i) = delete;
	RxRequestManager(const RxRequestManager& i) = delete;
	RxRequestManager& operator=(RxRequestManager&& i) = delete;
	RxRequestManager operator=(const RxRequestManager& i) = delete;

	~RxRequestManager() = default;

	rxcpp::observable<rxcpp::observable<std::string>> send_request(const std::string& url, std::string method, std::map<std::string, std::string> headers,
	                  const std::string& body);

	void set_curl_config(long timeout = 500) const;


private:
	RxRequestManager();
	std::vector<rxcpp::observable<HttpResponse>> m_requests;
	RxCurl* m_rx_curl;

	rxcpp::observable<std::string> response_message;
	rxcpp::observable<int> status_codes;
	std::string html;
};
