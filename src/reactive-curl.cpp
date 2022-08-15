#define CURL_STATICLIB

#include "rx-request-manager.hpp"

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main()
{
    YAML::Node config = YAML::LoadFile("../../../config.yml");
    json ex1 = json::parse(R"(
    {
        "pi": 3.141,
        "happy": true
    }
    )");

    const auto get_url = "www.google.com";
    const auto post_url = "https://httpbin.org/post";

    std::cout << "Hello World!" << std::endl;

    RxRequestManager::instance().set_curl_config(500);
    
    RxRequestManager::instance().send_request(get_url, "GET", {}, {}).subscribe(
    [&](const rxcpp::observable<std::string>& s)
    {
        s.subscribe(
        [&](const std::string& response_string)
        {
            std::cout << response_string << std::endl;
        });
    });

    RxRequestManager::instance().send_request(post_url, "POST", {}, {}).subscribe(
    [&](const rxcpp::observable<std::string>& s)
    {
        s.subscribe(
        [&](const std::string& response_string)
        {
            std::cout << response_string << std::endl;
            std::cout << "Goodbye World!" << std::endl;
            exit(0);
        });
    });

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
}
