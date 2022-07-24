#include "singleton-rx.hpp"

#include "rx-curl.hpp"

SingletonRx& SingletonRx::instance()
{
	static SingletonRx s_instance;
	return s_instance;
}

void SingletonRx::send_request(const std::string& url, std::string method, std::map<std::string, std::string> headers,
                               const std::string& body, std::promise<Response>* pr)
{
	const auto request = m_rx_curl->create(HttpRequest{url, std::move(method), std::move(headers), body})
		| rxcpp::rxo::map([&](const HttpResponse& r)
		{
			return r.body.complete;
		});
	request.subscribe(
		[&](const rxcpp::observable<std::string>& s)
		{
			response_message = s.sum();
		}, []
		{
		});
	response_message.subscribe(
		[&](const std::string& temp)
		{
			html = temp;
			if (pr)
			{
				const auto r{Response{m_rx_curl->m_http_status, html}};
				pr->set_value(r);
			}
		}, [&]
		{
		});
}

void SingletonRx::set_curl_config(const long timeout) const
{
	m_rx_curl->state->set_connection_timeout(timeout);
}

SingletonRx::SingletonRx() : m_rx_curl(create_rxcurl())
{
	std::cout << "Singleton RX Constructor" << std::endl;
	set_curl_config();
}
