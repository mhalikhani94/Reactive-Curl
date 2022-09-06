#pragma once
#include "types.hpp"

class EasyHandle {
public:
    using ptr_t = std::unique_ptr<EasyHandle>;
    using handle_t = decltype(curl_easy_init());

    EasyHandle() {
    }

    ~EasyHandle() {
        Close();
    }

    void Close() {
        if (handle_) {
            curl_easy_cleanup(handle_);
            handle_ = nullptr;
        }
    }

    operator handle_t () const noexcept { return handle_; }

private:
    handle_t handle_ = curl_easy_init();
};