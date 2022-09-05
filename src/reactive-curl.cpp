// #include "rx-request-manager.hpp"

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <chrono>
#include <thread>
#include "rx-curl2.hpp"

int main()
{
    const auto get_url = "www.google.com";
    const auto post_url = "https://httpbin.org/post";

    auto rx_curl2_instance = create_rxcurl2();
    rx_curl2_instance->create(HttpRequest{post_url, HttpRequestMethod::kPost, {}, {}}).subscribe(
        [=](const HttpResponse& r)
        {
            std::cout << r.response_body << std::endl;
            exit(0);
        }, [] ()
        {
        }
    );

    rx_curl2_instance->create(HttpRequest{get_url, HttpRequestMethod::kGet, {}, {}}).subscribe(
        [=](const HttpResponse& r)
        {
            std::cout << r.response_body << std::endl;
            exit(0);
        }, [] ()
        {
        }
    );

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
}

//subscibe_on(sceduler)
//observe_on(computation)
////mapping//observe_on(main)
//subscribe on main