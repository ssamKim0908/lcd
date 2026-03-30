#pragma once
#include "pch.h"
#include "transport_interface.hpp"

class Fd_spi : public Fd_transport, public ITransport_write, public ITransport_ioctl
{
public:
    Fd_spi(const std::string& device, int flags) : Fd_transport(device, flags) {}

    void Write(const uint8_t* buffer, uint32_t length) override;
    void Ioctl(uint32_t flag, void* arg) override;
};