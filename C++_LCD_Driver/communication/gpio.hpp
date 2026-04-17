#pragma once
#include "../interface/ICommunication.hpp"
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

class FdGpio : public IRead
{
private:
    FdObject FdT;
public:
    FdGpio(const std::string& device, int flags);
    void read(span<std::byte> buffer) override;
};