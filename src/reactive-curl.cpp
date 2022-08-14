#define CURL_STATICLIB

#include "singleton-rx.hpp"

// #include "yaml-cpp/yaml.h"

int main()
{
    // YAML::Node config = YAML::LoadFile("../config.yml");

    const auto get_url = "www.google.com";
    const auto pst_url = "https://httpbin.org/post";

    std::promise<Response> promise_val{};
    std::vector<std::future<Response>> future_vec;
    future_vec.emplace_back(promise_val.get_future());
    SingletonRx::instance().set_curl_config(500);
    SingletonRx::instance().send_request(get_url, "GET", {}, {}, &promise_val);
    //*
    if(future_vec.size() > 0)
        future_vec.at(0).wait();
    if(future_vec.at(0).valid())
    {
        const auto res = future_vec.at(0).get();
        std::cout << "Http Status: " << res.http_status << std::endl;
        std::cout << "Http Body: " << res.http_response << std::endl;
        promise_val = std::promise<Response>{};
        future_vec.emplace_back(promise_val.get_future());
        if(res.http_status == 200)
        {
            SingletonRx::instance().send_request(pst_url, "POST", {}, {}, &promise_val);
            future_vec.at(1).wait();
            if(future_vec.at(1).valid())
            {
                const auto res = future_vec.at(1).get();
                std::cout << "Http Status: " << res.http_status << std::endl;
                std::cout << "Http Body: " << res.http_response << std::endl;
                return 0;
            }
        }
        else
        {
            std::cerr << "Bad Http Status. Https status: " << res.http_status << std::endl;
        }
    }
    // */

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
}
