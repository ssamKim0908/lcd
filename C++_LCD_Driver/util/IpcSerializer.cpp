#include "IpcSerializer.hpp"
#include <cstring>
#include <stdexcept>

namespace util::ipc
{
// ===== Writer =====

void Writer::put_u8(std::vector<std::byte>& buf, uint8_t x) const
{
    buf.push_back(static_cast<std::byte>(x));
}

void Writer::put_u16(std::vector<std::byte>& buf, uint16_t x) const
{
    const auto* p = reinterpret_cast<const std::byte*>(&x);
    buf.insert(buf.end(), p, p + sizeof(x));
}

void Writer::put_ui32(std::vector<std::byte>& buf, uint32_t x) const
{
    const auto* p = reinterpret_cast<const std::byte*>(&x);
    buf.insert(buf.end(), p, p + sizeof(x));
}

void Writer::put_i8(std::vector<std::byte>& buf, int8_t x) const
{
    buf.push_back(static_cast<std::byte>(static_cast<uint8_t>(x)));
}

void Writer::put_i16(std::vector<std::byte>& buf, int16_t x) const
{
    const auto* p = reinterpret_cast<const std::byte*>(&x);
    buf.insert(buf.end(), p, p + sizeof(x));
}

void Writer::put_i32(std::vector<std::byte>& buf, int32_t x) const
{
    const auto* p = reinterpret_cast<const std::byte*>(&x);
    buf.insert(buf.end(), p, p + sizeof(x));
}

void Writer::put_text_size(std::vector<std::byte>& buf, util::font::TextSize size) const
{
    put_u8(buf, static_cast<uint8_t>(size));
}

void Writer::put_string(std::vector<std::byte>& buf, std::string_view s) const
{
    put_u8(buf, static_cast<uint8_t>(s.size()));
    const auto* p = reinterpret_cast<const std::byte*>(s.data());
    buf.insert(buf.end(), p, p + s.size());
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
    ensure(sizeof(uint32_t));
    uint32_t v{};
    std::memcpy(&v, cur_, sizeof(v));
    cur_ += sizeof(v);
    return v;
}

int8_t Reader::get_i8()
{
    ensure(sizeof(int8_t));
    int8_t v = static_cast<int8_t>(static_cast<uint8_t>(*cur_));
    cur_ += sizeof(int8_t);
    return v;
}

int16_t Reader::get_i16()
{
    ensure(sizeof(int16_t));
    int16_t v{};
    std::memcpy(&v, cur_, sizeof(v));
    cur_ += sizeof(v);
    return v;
}

int32_t Reader::get_i32()
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
