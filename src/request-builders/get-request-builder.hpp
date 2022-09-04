// #ifndef GET_REQUEST_BUILDER_HPP
// #define GET_REQUEST_BUILDER_HPP

// #include "request-builder-base.hpp"
// #include "../types.hpp"
// #include "../rx-curl.hpp"

// class GetRequestBuilder : public RequestBuilderBase
// {
// public:
//     GetRequestBuilder(const std::string& url);

//     GetRequestBuilder& add_header(const std::string& key, const std::string& value) override;
//     GetRequestBuilder& add_headers(const std::map<std::string, std::string>& headers) override;
//     GetRequestBuilder& add_headers(/*Object?*/) override;

//     GetRequestBuilder& add_body_parameter(const std::string& body) override;
//     rxcpp::observable<HttpResponse> exec();


// private:
//     std::string m_url;
//     HttpRequestMethod m_method;
//     std::string m_body;
//     std::map<std::string, std::string> m_headers;

// };

// #endif //GET_REQUEST_BUILDER_HPP