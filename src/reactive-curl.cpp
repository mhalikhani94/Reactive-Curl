// #include "rx-request-manager.hpp"

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <chrono>
#include <thread>
#include "http-client/http-client.hpp"

int main()
{
    const auto get_url = "https://httpbin.org/get";
    const auto post_url = "https://httpbin.org/post";

    HttpClient client;
    client.build()->Get(get_url)
    .add_curl_option(CURLOPT_VERBOSE, 0L)
    .process_response([&](const HttpResponse& result)
    {
       if (result.is_ok())
            {
                try
                {
                    std::cout << "Result Body: " << result.body << std::endl;
                    std::cout << "Result headers: " << result.header << std::endl;
                } catch (const std::exception& ex) {
                    std::cout << ex.what() << std::endl;
                    return;
                }
                
            } else {
                std::cout << "Request failed: " << result.error_msg << std::endl
                    << "HTTP code: " << result.http_response_code << std::endl;
            }
    })
    .send_request();

    client.build()->Post(post_url)
        .accept_json()
        .add_json_body("")
        .add_header("Cache-Control: no-cache, no-store, must-revalidate")
        .add_header("Pragma: no-cache")
        .add_header("Expires: 0")
        .process_response([&](const HttpResponse& result)
        {
            if (result.is_ok())
            {
                try
                {
                    std::cout << "Result Body: " << result.body << std::endl;
                    std::cout << "Result headers: " << result.header << std::endl;
                } catch (const std::exception& ex) {
                    std::cout << ex.what() << std::endl;
                    return;
                }
                
            } else {
                std::cout << "Request failed: " << result.error_msg << std::endl
                    << "HTTP code: " << result.http_response_code << std::endl;
            }
        })
        .send_request();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
}
