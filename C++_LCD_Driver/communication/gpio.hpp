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

class Fd_Gpio : public IRead
{
private:
    Fd_Object fd_t;
public:
    Fd_Gpio(const std::string& device, int flags);
    void read(span<uint8_t> buffer) override;
};