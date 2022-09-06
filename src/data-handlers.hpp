#pragma once
#include "types.hpp"

struct DataHandlerBase {
    virtual ~DataHandlerBase() = default;
};

template <typename T>
struct InDataHandler : public DataHandlerBase{
    InDataHandler(T& data) : data_{data} {
    }

    static size_t write_callback(char *ptr, size_t size, size_t nitems, void *userdata) {
        assert(userdata);
        auto self = reinterpret_cast<InDataHandler *>(userdata);
        const auto bytes = size * nitems;
        if (bytes > 0) {
            std::copy(ptr, ptr + bytes, std::back_inserter(self->data_));
        }
        return bytes;
    }

    T& data_;
};

template <typename T>
struct OutDataHandler : public DataHandlerBase {
    OutDataHandler() = default;
    OutDataHandler(const T& v) : data_{v} {}
    OutDataHandler(T&& v) : data_{std::move(v)} {}

    static size_t read_callback(char *bufptr, size_t size, size_t nitems, void *userdata) {
        assert(userdata);
        OutDataHandler *self = reinterpret_cast<OutDataHandler *>(userdata);
        const auto bytes = size * nitems;
        auto out_bytes = std::min<size_t>(bytes, (self->data_.size() - self->sendt_bytes_));
        std::copy(self->data_.cbegin() + self->sendt_bytes_,
                    self->data_.cbegin() + (self->sendt_bytes_ + out_bytes),
                    bufptr);
        self->sendt_bytes_ += out_bytes;

        return out_bytes;
    }

    T data_;
    size_t sendt_bytes_ = 0;
};