#pragma once
#include "pch.h"
#include "transport_interface.hpp"
#include <linux/gpio.h>

enum class GpioDirection
{
    Input,
    Output
};

enum class GpioValue
{
    Low,
    High
};

class Fd_gpio : public Fd_transport, public ITransport_read, public ITransport_ioctl
{
public:
    Fd_gpio(const std::string& device, int flags) : Fd_transport(device, flags) {}
    void read() override;
    void ioctl() override;
};