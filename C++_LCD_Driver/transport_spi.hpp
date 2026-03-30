#pragma once
#include "pch.h"
#include "transport_interface.hpp"

class Fd_Spi : public ITransport_Write, public ITransport_Ioctl
{
private:
    Fd_Transport fd_t;
public:
    Fd_Spi(const std::string& device, int flags) : fd_t{device, flags} {};

    void write(span<uint8_t> buffer) override;
    void ioctl(uint32_t flag, void* arg) override;
};