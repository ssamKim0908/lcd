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

class gpio : public IRead
{
private:
    int fd = -1;
public:
    gpio(const std::string& device, int flags);
    ~gpio();

    gpio(const gpio&)            = delete;
    gpio& operator=(const gpio&) = delete;

    void read(span<std::byte> buffer) override;
};
