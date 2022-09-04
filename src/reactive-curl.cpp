// #include "rx-request-manager.hpp"

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <chrono>
#include <thread>
#include "rx-curl.hpp"

int main()
{
    YAML::Node node = YAML::Load("[1, 2, 3]");
    json ex1 = json::parse(R"(
    {
        "pi": 3.141,
        "happy": true
    }
    )");
    const auto get_url = "www.google.com";
    const auto post_url = "https://httpbin.org/post";

    std::cout << "Hello World!" << std::endl;

    // RxRequestManager::instance().set_curl_config(500);

    // RxRequestManager::instance().send_request(post_url, HttpRequestMethod::kPost, {}, {}).subscribe(
    // [&](const std::string& s)
    // {
    //     std::cout << s << std::endl;
    // },
    // [](){}
    // );

    // RxRequestManager::instance().send_request(get_url, HttpRequestMethod::kGet, {}, {}).subscribe(
    // [&](const std::string& s)
    // {
    //     std::cout << s << std::endl;
    //     std::cout << "Goodbye World!" << std::endl;
    //     // exit(0);
    // },
    // [](){}
    // );

    auto rx_curl_instance = create_rxcurl();
    const auto post_request = rx_curl_instance->create(HttpRequest{post_url, HttpRequestMethod::kPost, {}, {}});

    post_request.subscribe(
    [=](const HttpResponse& r)
    {
        std::cout << "Curl  Code:" << r.code() << std::endl;
        std::cout << "httpStatus:" << r.httpStatus() << std::endl;
        std::cout << "http Error:" << r.httpError() << std::endl;
    }, [] ()
    {
    });

    const auto get_request = rx_curl_instance->create(HttpRequest{get_url, HttpRequestMethod::kGet, {}, {}});

    get_request.subscribe(
    [=](const HttpResponse& r)
    {
        std::cout << "Curl  Code:" << r.code() << std::endl;
        std::cout << "httpStatus:" << r.httpStatus() << std::endl;
        std::cout << "http Error:" << r.httpError() << std::endl;
    }, [] ()
    {
    });

    // RxRequestManager::instance().send_request(get_url, HttpRequestMethod::kGet, {}, {}).subscribe(
    // [&](const rxcpp::observable<std::string>& s)
    // {
    //     s.subscribe(
    //     [&](const std::string& response_string)
    //     {
    //         // std::cout << response_string << std::endl;
    //         std::cout << "Goodbye World!" << std::endl;
    //         exit(0);
    //     });
    // });

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
}
