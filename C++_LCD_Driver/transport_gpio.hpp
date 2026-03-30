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

class Fd_Gpio : public ITransport_Read, public ITransport_Ioctl
{
private:
    Fd_Transport fd_t;
public:
    Fd_Gpio(const std::string& device, int flags) : fd_t(device, flags) {};
    void Read(uint8_t* buffer, uint32_t length) override;
    void Ioctl(uint32_t flag, void* arg) override;
};