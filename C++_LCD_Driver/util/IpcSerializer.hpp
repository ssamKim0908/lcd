#pragma once
#include "../util/span.hpp"
#include "../util/Font.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace util::ipc
{

class Writer
{
private:
    std::vector<std::byte> buf_;
public:
    Writer() = default;

    void put_u8         (uint8_t  x);
    void put_u16        (uint16_t x);
    void put_ui32       (uint32_t  x);
    void put_text_size  (util::font::TextSize size);
    void put_string     (std::string_view s);
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
    uint32_t             get_ui32     ();
    util::font::TextSize get_text_size();
    std::string          get_string   ();

    bool empty() const { return cur_ == end_; }

private:
    void ensure(std::size_t n) const;
};

}
