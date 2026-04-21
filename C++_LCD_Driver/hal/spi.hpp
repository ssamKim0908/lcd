#pragma once
#include "ICommunication.hpp"

class spi : public IWrite
{
private:
    int fd = -1;

    uint8_t  mode;
    uint8_t  bits;
    uint32_t speed;
    uint8_t  lsb;
public:
    spi(const std::string& device, int flags);
    ~spi();

    spi(const spi&)            = delete;
    spi& operator=(const spi&) = delete;

    void write(span<const std::byte> buffer) override;
};
