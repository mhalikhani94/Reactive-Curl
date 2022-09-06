#pragma once
#include "types.hpp"

class Exception : public std::runtime_error {
public:
    Exception(const std::string& msg) : runtime_error(msg) {}
};

class CurlException : public Exception {
public:
    CurlException(const std::string msg, const CURLcode err)
        : Exception(msg + '(' + std::to_string(err) + "): " + curl_easy_strerror(err))
        , err_{err}
        {}

        CurlException(const std::string msg, const CURLMcode err)
        : Exception(msg + '(' + std::to_string(err) + "): " + curl_multi_strerror(err))
        , err_{err}
        {}

    int getErrorCode() const noexcept { return err_; }

private:
    const int err_;
};