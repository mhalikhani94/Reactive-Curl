// #include "rx-request-manager.hpp"

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <chrono>
#include <thread>
#include "rx-curl2.hpp"
#include "mh-http-client.hpp"

int main()
{
    const auto get_url = "www.google.com";
    const auto post_url = "https://httpbin.org/post";

    restincurl::Client client;
    // client.Build()->Get("www.google.com")
    // .Option(CURLOPT_VERBOSE, 1L)
    // .WithCompletion([&](const restincurl::Result& result) {
    //    std::cout << result.body << std::endl;
    // })
    // .ExecuteSynchronous();

    client.Build()->Post("https://mh-rx-test.free.beeceptor.com")
        .AcceptJson()
        .WithCompletion([&](const restincurl::Result& result) {
            if (result.isOk()) {
                try {
                    const auto j1 = json::parse(result.body);
                    std::cout << j1.dump() << std::endl;
                    std::cout << "Result headers: " << result.header << std::endl;
                } catch (const std::exception& ex) {
                    std::cout << ex.what() << std::endl;
                    return;
                }
                
            } else {
                std::cout << "Request failed: " << result.msg << std::endl
                    << "HTTP code: " << result.http_response_code << std::endl;
            }
        })
        .ExecuteSynchronous();

/*
    HttpClient* client = new HttpClient.
    client.post("URL")
        .add_headers("Header1")
        .add_headers("Header2")
        .add_headers("Header3")
        .add_body("body1")
        .add_body("body2")
        .build();
// */











    // auto rx_curl2_instance = create_rxcurl2();
    // rx_curl2_instance->create(HttpRequest{post_url, HttpRequestMethod::kPost, {}, {}}).subscribe(
    //     [=](const HttpResponse& r)
    //     {
    //         std::cout << r.response_body << std::endl;
    //         exit(0);
    //     }, [] ()
    //     {
    //     }
    // );

    // rx_curl2_instance->create(HttpRequest{get_url, HttpRequestMethod::kGet, {}, {}}).subscribe(
    //     [=](const HttpResponse& r)
    //     {
    //         std::cout << r.response_body << std::endl;
    //         exit(0);
    //     }, [] ()
    //     {
    //     }
    // );

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
}

//subscibe_on(sceduler)
//observe_on(computation)
////mapping//observe_on(main)
//subscribe on main