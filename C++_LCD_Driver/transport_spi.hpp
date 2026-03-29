#pragma once
#include <linux/spi/spidev.h>
#include "pch.h"

class Fd_spi : public Fd_transport, public ITransport_write, public ITransport_ioctl
{
public:
    Fd_spi(const std::string& device, int flags) : Fd_transport(device, flags) {}

    void write() override;
    void ioctl() override;
};