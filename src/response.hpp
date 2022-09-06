#pragma once
#include "types.hpp"

struct Result {
    Result() = default;
    Result(const CURLcode& code) {
        curl_code = code;
        msg = curl_easy_strerror(code);
    }
    
    bool isOk() const noexcept {
        if (curl_code == CURLE_OK) {
            if ((http_response_code >= 200) && (http_response_code < 300)) {
                return true;
            }
        }
        
        return false;
    }

    CURLcode curl_code = {};
    
    long http_response_code = {};
    
    std::string msg;
    
    std::string body;
    std::string header;
};