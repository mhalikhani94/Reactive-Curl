#pragma once
#include "types.hpp"

class Options {
public:
    Options(EasyHandle& eh) : eh_{eh} {}

    template <typename T>
    Options& Set(const CURLoption& opt, const T& value) {
        const auto ret = curl_easy_setopt(eh_, opt, value);
        if (ret) {
            throw CurlException(
                std::string("Setting option ") + std::to_string(opt), ret);
        }
        return *this;
    }

    Options& Set(const CURLoption& opt, const std::string& value) {
        return Set(opt, value.c_str());
    }

private:
    EasyHandle& eh_;
};