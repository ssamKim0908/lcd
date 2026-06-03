#pragma once
#include "../util/span.hpp"
#include "../util/Font.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace util::ipc
{

class Writer
{
public:
    void clear();

    void put_u8        (uint8_t  x);
    void put_u16       (uint16_t x);
    void put_u32       (uint32_t x);
    void put_i8        (int8_t   x);
    void put_i16       (int16_t  x);
    void put_i32       (int32_t  x);
    void put_text_size (util::font::TextSize s);
    void put_string    (const std::string&   s);

    util::Span<const std::byte> bytes() const { return { buffer_.data(), buffer_.size() }; }

private:
    std::vector<std::byte> buffer_;
};

class Reader
{
private:
    const std::byte* cur_;
    const std::byte* end_;
public:
    Reader(const std::byte* data, std::size_t size)
        : cur_(data), end_(data + size) {}

    Reader(const std::byte* start, const std::byte* end)
        : cur_(start), end_(end) {}

    uint8_t              get_u8       ();
    uint16_t             get_u16      ();
    uint32_t             get_u32      ();
    int8_t               get_i8       ();
    int16_t              get_i16      ();
    int32_t              get_i32      ();
    util::font::TextSize get_text_size();
    std::string          get_string   ();

    bool empty() const { return cur_ == end_; }

private:
    void ensure(std::size_t n) const;
};

}
