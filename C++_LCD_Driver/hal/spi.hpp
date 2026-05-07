#pragma once
#include "../util/span.hpp"
#include <cstdint>
#include <string>

class Spi
{
private:
    int fd = -1;

    uint8_t  mode;
    uint8_t  bits;
    uint32_t speed;
    uint8_t  lsb;
public:
    Spi(const std::string& device, int flags);
    ~Spi();

    Spi(const Spi&)            = delete;
    Spi& operator=(const Spi&) = delete;

    void write(util::Span<const std::byte> buffer);
};
