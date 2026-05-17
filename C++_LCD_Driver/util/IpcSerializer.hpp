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
public:
    void put_u8        (std::vector<std::byte>& buf, uint8_t  x)             const;
    void put_u16       (std::vector<std::byte>& buf, uint16_t x)             const;
    void put_ui32      (std::vector<std::byte>& buf, uint32_t x)             const;
    void put_i8        (std::vector<std::byte>& buf, int8_t  x)              const;
    void put_i16       (std::vector<std::byte>& buf, int16_t x)              const;
    void put_i32       (std::vector<std::byte>& buf, int32_t x)              const;
    void put_text_size (std::vector<std::byte>& buf, util::font::TextSize s) const;
    void put_string    (std::vector<std::byte>& buf, std::string_view s)     const;
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
