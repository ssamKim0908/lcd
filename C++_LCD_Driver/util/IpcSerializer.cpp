#include "IpcHelper.hpp"
#include <cstring>
#include <stdexcept>

namespace util::ipc
{
// ===== Writer : private =====

// ===== Writer : public =====

void Writer::put_u8(uint8_t x)
{
    buf_.push_back(static_cast<std::byte>(x));
}

void Writer::put_u16(uint16_t x)
{
    const auto* p = reinterpret_cast<const std::byte*>(&x);
    buf_.insert(buf_.end(), p, p + sizeof(x));
}

void Writer::put_ui32(uint32_t x)
{
    const auto* p = reinterpret_cast<const std::byte*>(&x);
    buf_.insert(buf_.end(), p, p + sizeof(x));
}

void Writer::put_text_size(util::font::TextSize size)
{
    put_u8(static_cast<uint8_t>(size));
}

void Writer::put_string(std::string_view s)
{
    put_u16(static_cast<uint16_t>(s.size()));
    const auto* p = reinterpret_cast<const std::byte*>(s.data());
    buf_.insert(buf_.end(), p, p + s.size());
}


// ===== Reader : private =====

void Reader::ensure(std::size_t n) const
{
    if (static_cast<std::size_t>(end_ - cur_) < n)
        throw std::runtime_error("Reader: short read");
}

// ===== Reader : public =====

uint8_t Reader::get_u8()
{
    ensure(sizeof(uint8_t));
    uint8_t v = static_cast<uint8_t>(*cur_);
    cur_ += sizeof(uint8_t);
    return v;
}

uint16_t Reader::get_u16()
{
    ensure(sizeof(uint16_t));
    uint16_t v{};
    std::memcpy(&v, cur_, sizeof(v));
    cur_ += sizeof(v);
    return v;
}

uint32_t Reader::get_ui32()
{
    ensure(sizeof(int32_t));
    int32_t v{};
    std::memcpy(&v, cur_, sizeof(v));
    cur_ += sizeof(v);
    return v;
}

util::font::TextSize Reader::get_text_size()
{
    return static_cast<util::font::TextSize>(get_u8());
}

std::string Reader::get_string()
{
    auto len = get_u16();
    ensure(len);
    std::string s(reinterpret_cast<const char*>(cur_), len);
    cur_ += len;
    return s;
}

}
