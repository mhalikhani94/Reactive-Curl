#pragma once
#include "types.hpp"

class Client {

public:
    Client(const bool init = true) {
        if (init) {
            static std::once_flag flag;
            std::call_once(flag, [] {
                // RESTINCURL_LOG("One time initialization of curl.");
                curl_global_init(CURL_GLOBAL_DEFAULT);
            });
        }
    }

    virtual ~Client() {
    }

    std::unique_ptr<RequestBuilder> Build()
    {
        return std::make_unique<RequestBuilder>();
    }
private:
};