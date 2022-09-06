#pragma once
#include "types.hpp"

class RequestBuilder {
    static size_t write_callback(char *ptr, size_t size, size_t nitems, void *userdata) {
        const auto bytes = size * nitems;
        return bytes;
    }

public:
    using ptr_t = std::unique_ptr<RequestBuilder>;
    RequestBuilder(
    )
    : request_{std::make_unique<Request>()}
    , options_{std::make_unique<class Options>(request_->GetEasyHandle())}
    {}

    ~RequestBuilder() {
    }

protected:
    RequestBuilder& Prepare(RequestType rt, const std::string& url) {
        assert(request_type_ == RequestType::INVALID);
        assert(!is_built_);
        request_type_  = rt;
        url_ = url;
        return *this;
    }

public:
    RequestBuilder& Get(const std::string& url) {
        return Prepare(RequestType::GET, url);
    }

    RequestBuilder& Head(const std::string& url) {
        return Prepare(RequestType::HEAD, url);
    }

    RequestBuilder& Post(const std::string& url) {
        return Prepare(RequestType::POST, url);
    }

    RequestBuilder& Put(const std::string& url) {
        return Prepare(RequestType::PUT, url);
    }

    RequestBuilder& Patch(const std::string& url) {
        return Prepare(RequestType::PATCH, url);
    }

    RequestBuilder& Delete(const std::string& url) {
        return Prepare(RequestType::DELETE, url);
    }

    RequestBuilder& Options(const std::string& url) {
        return Prepare(RequestType::OPTIONS, url);
    }

    RequestBuilder& Header(const char *value) {
        assert(value);
        assert(!is_built_);
        request_->GetHeaders() = curl_slist_append(request_->GetHeaders(), value);
        return *this;
    }

    RequestBuilder& Header(const std::string& name,
                            const std::string& value) {
        const auto v = name + ": " + value;
        return Header(v.c_str());
    }

    RequestBuilder& WithJson() {
        return Header("Content-type: Application/json; charset=utf-8");
    }
    
    RequestBuilder& WithJson(std::string body) {
        WithJson();
        return SendData(std::move(body));
    }

    RequestBuilder& AcceptJson() {
        return Header("Accept: Application/json");
    }

    template <typename T>
    RequestBuilder& Option(const CURLoption& opt, const T& value) {
        assert(!is_built_);
        options_->Set(opt, value);
        return *this;
    }

    RequestBuilder& Trace(bool enable = true) {
        if (enable) {
            Option(CURLOPT_VERBOSE, 1L);
        } else {
            Option(CURLOPT_VERBOSE, 0L);
        }
        
        return *this;
    }

    RequestBuilder& RequestTimeout(const long timeout) {
        request_timeout_ = timeout;
        return *this;
    }

    RequestBuilder& ConnectTimeout(const long timeout) {
        connect_timeout_ = timeout;
        return *this;
    }

    template <typename T>
    RequestBuilder& SendData(OutDataHandler<T>& dh) {
        assert(!is_built_);
        options_->Set(CURLOPT_READFUNCTION, dh.read_callback);
        options_->Set(CURLOPT_READDATA, &dh);
        have_data_out_ = true;
        return *this;
    }

    template <typename T>
    RequestBuilder& SendData(T data) {
        assert(!is_built_);
        auto handler = std::make_unique<OutDataHandler<T>>(std::move(data));
        auto& handler_ref = *handler;
        request_->SetDefaultOutHandler(std::move(handler));
        return SendData(handler_ref);
    }

    template <typename T>
    RequestBuilder& StoreData(InDataHandler<T>& dh) {
        assert(!is_built_);
        options_->Set(CURLOPT_WRITEFUNCTION, dh.write_callback);
        options_->Set(CURLOPT_WRITEDATA, &dh);
        have_data_in_ = true;
        return *this;
    }

    template <typename T>
    RequestBuilder& StoreData(T& data) {
        assert(!is_built_);
        auto handler = std::make_unique<InDataHandler<T>>(data);
        auto& handler_ref = *handler;
        request_->SetDefaultInHandler(std::move(handler));
        return StoreData(handler_ref);
    }

    template <typename T>
    RequestBuilder& StoreHeader(InDataHandler<T>& dh) {
        assert(!is_built_);
        options_->Set(CURLOPT_HEADERFUNCTION, dh.write_callback);
        options_->Set(CURLOPT_HEADERDATA, &dh);
        have_header_in_ = true;
        return *this;
    }

    template <typename T>
    RequestBuilder& StoreHeader(T& data) {
        assert(!is_built_);
        auto handler = std::make_unique<InDataHandler<T>>(data);
        auto& handler_ref = *handler;
        request_->SetDefaultHeaderHandler(std::move(handler));
        return StoreHeader(handler_ref);
    }

    RequestBuilder& IgnoreIncomingData() {
        options_->Set(CURLOPT_WRITEFUNCTION, write_callback);
        have_data_in_ = true;
        return *this;
    }

    RequestBuilder& WithCompletion(completion_fn_t fn) {
        assert(!is_built_);
        completion_ = std::move(fn);
        return *this;
    }

    RequestBuilder& BasicAuthentication(const std::string& name,
                                        const std::string& passwd) {
        assert(!is_built_);

        if (!name.empty() && !passwd.empty()) {
            options_->Set(CURLOPT_USERNAME, name.c_str());
            options_->Set(CURLOPT_PASSWORD, passwd.c_str());
        }

        return *this;
    }

    RequestBuilder& SetReadHandler(size_t (*handler)(char *, size_t , size_t , void *), void *userdata) {
        options_->Set(CURLOPT_READFUNCTION, handler);
        options_->Set(CURLOPT_READDATA, userdata);
        have_data_out_ = true;
        return *this;
    }

    RequestBuilder& SetWriteHandler(size_t (*handler)(char *, size_t , size_t , void *), void *userdata) {
        options_->Set(CURLOPT_WRITEFUNCTION,handler);
        options_->Set(CURLOPT_WRITEDATA, userdata);
        have_data_in_ = true;
        return *this;
    }

    void Build() {
        if (!is_built_) {
            // Set up Data Handlers
            if (!have_data_in_) {
                // Use a default std::string. We expect json anyway...
                this->StoreData(request_->getDefaultInBuffer());
            }

            if (!have_header_in_) {
                // Use a default std::string. We expect json anyway...
                // this->StoreData(request_->getDefaultInBuffer());
            }

            if (have_data_out_) {
                options_->Set(CURLOPT_UPLOAD, 1L);
            }

            if (request_timeout_ >= 0) {
                options_->Set(CURLOPT_TIMEOUT_MS, request_timeout_);
            }

            if (connect_timeout_ >= 0) {
                options_->Set(CURLOPT_CONNECTTIMEOUT_MS, connect_timeout_);
            }

            // Set headers
            if (request_->GetHeaders()) {
                options_->Set(CURLOPT_HTTPHEADER, request_->GetHeaders());
            }

            options_->Set(CURLOPT_URL, url_);

            request_->Prepare(request_type_, std::move(completion_));
            is_built_ = true;
        }
    }

    void ExecuteSynchronous() {
        Build();
        request_->Execute();
    }

private:
    std::unique_ptr<Request> request_;
    std::unique_ptr<class Options> options_;
    std::string url_;
    RequestType request_type_ = RequestType::INVALID;
    bool have_data_in_ = false;
    bool have_header_in_ = false;
    bool have_data_out_ = false;
    bool is_built_ = false;
    completion_fn_t completion_;
    long request_timeout_ = 10000L; // 10 seconds
    long connect_timeout_ = 3000L; // 1 second
};
