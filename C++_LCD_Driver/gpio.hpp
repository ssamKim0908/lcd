#pragma once
#include "pch.h"
#include "communication_interface.hpp"
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

class Fd_Gpio : public IRead, public IIoctl
{
private:
    Fd_Object fd_t;
public:
    Fd_Gpio(const std::string& device, int flags) : fd_t(device, flags) {};
    void read(span<uint8_t> buffer) override;
    void ioctl(uint32_t flag, void* arg) override;
};