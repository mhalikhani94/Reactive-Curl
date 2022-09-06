#pragma once
#include "types.hpp"

class Request {
public:
    using ptr_t = std::unique_ptr<Request>;

    Request()
    : eh_{std::make_unique<EasyHandle>()}
    {
    }

    Request(EasyHandle::ptr_t&& eh)
    : eh_{std::move(eh)}
    {
    }

    ~Request() {
        if (headers_) {
            curl_slist_free_all(headers_);
        }
    }

    void Prepare(const RequestType rq, completion_fn_t completion) {
        request_type_ = rq;
        SetRequestType();
        completion_ = std::move(completion);
    }

    // Synchronous execution.
    void Execute() {
        const auto result = curl_easy_perform(*eh_);
        CallCompletion(result);
    }

    void Complete(CURLcode cc, const CURLMSG& /*msg*/) {
        CallCompletion(cc);
    }

    EasyHandle& GetEasyHandle() noexcept { assert(eh_); return *eh_; }
    RequestType GetRequestType() noexcept { return request_type_; }

    void SetDefaultInHandler(std::unique_ptr<DataHandlerBase> ptr) {
        default_in_handler_ = std::move(ptr);
    }

    void SetDefaultOutHandler(std::unique_ptr<DataHandlerBase> ptr) {
        default_out_handler_ = std::move(ptr);
    }

    void SetDefaultHeaderHandler(std::unique_ptr<DataHandlerBase> ptr) {
        default_header_handler_ = std::move(ptr);
    }

    using headers_t = curl_slist *;
    headers_t& GetHeaders() {
        return headers_;
    }

    std::string& getDefaultInBuffer() {
        return default_data_buffer_;
    }

    std::string& getDefaultHeaderInBuffer() {
        return default_header_buffer_;
    }

private:
    void CallCompletion(CURLcode cc) {
        Result result(cc);

        curl_easy_getinfo (*eh_, CURLINFO_RESPONSE_CODE,
                            &result.http_response_code);
        if (completion_) {
            if (!default_data_buffer_.empty()) {
                result.body = std::move(default_data_buffer_);
            }
            if (!default_header_buffer_.empty()) {
                result.header = std::move(default_header_buffer_);
            }
            completion_(result);
        }
    }

    void SetRequestType() {
        switch(request_type_) {
            case RequestType::GET:
                curl_easy_setopt(*eh_, CURLOPT_HTTPGET, 1L);
                break;
            case RequestType::PUT:
                headers_ = curl_slist_append(headers_, "Transfer-Encoding: chunked");
                curl_easy_setopt(*eh_, CURLOPT_UPLOAD, 1L);
                break;
            case RequestType::POST:
                headers_ = curl_slist_append(headers_, "Transfer-Encoding: chunked");
                curl_easy_setopt(*eh_, CURLOPT_UPLOAD, 0L);
                curl_easy_setopt(*eh_, CURLOPT_POST, 1L);
                break;
            case RequestType::HEAD:
                curl_easy_setopt(*eh_, CURLOPT_NOBODY, 1L);
                break;
            case RequestType::OPTIONS:
                curl_easy_setopt(*eh_, CURLOPT_CUSTOMREQUEST, "OPTIONS");
                break;
            case RequestType::PATCH:
                headers_ = curl_slist_append(headers_, "Transfer-Encoding: chunked");
                curl_easy_setopt(*eh_, CURLOPT_CUSTOMREQUEST, "PATCH");
                break;
            case RequestType::DELETE:
                curl_easy_setopt(*eh_, CURLOPT_CUSTOMREQUEST, "DELETE");
                break;
            default:
                throw Exception("Unsupported request type" + std::to_string(static_cast<int>(request_type_)));
        }
    }

    EasyHandle::ptr_t eh_;
    RequestType request_type_ = RequestType::INVALID;
    completion_fn_t completion_;
    std::unique_ptr<DataHandlerBase> default_out_handler_;
    std::unique_ptr<DataHandlerBase> default_in_handler_;
    std::unique_ptr<DataHandlerBase> default_header_handler_;
    headers_t headers_ = nullptr;
    std::string default_data_buffer_;
    std::string default_header_buffer_;
};