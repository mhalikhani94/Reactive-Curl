// #include "get-request-builder.hpp"

// GetRequestBuilder::GetRequestBuilder(const std::string& url) : m_url(url)
// , m_method(HttpRequestMethod::kGet)
// {

// }

// GetRequestBuilder& GetRequestBuilder::add_header(const std::string& key, const std::string& value)
// {
//     m_headers.insert({key,value});
//     return *this;
// }

// GetRequestBuilder& GetRequestBuilder::add_headers(const std::map<std::string, std::string>& headers)
// {
//     m_headers.insert(headers.begin(),headers.end());
//     return *this;
// }

// GetRequestBuilder& GetRequestBuilder::add_headers(/*Object?*/)
// {
//     return *this;
// }

// GetRequestBuilder& GetRequestBuilder::add_body_parameter(const std::string& body)
// {
//     m_body = body;
//     return *this;
// }

// rxcpp::observable<HttpResponse> GetRequestBuilder::exec()
// {
//     auto factory = create_rxcurl();
//     ret
// }
