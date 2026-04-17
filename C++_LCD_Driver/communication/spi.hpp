#pragma once
#include "ICommunication.hpp"

class FdSpi : public IWrite
{
private:
    FdObject FdT;
    
    uint8_t  mode;
    uint8_t  bits;
    uint32_t speed;
    uint8_t  lsb;
public:
    FdSpi(const std::string& device, int flags);

    void write(span<const std::byte> buffer) override;
};