#pragma once
#include "../interface/ICommunication.hpp"

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

enum class InputKey : uint32_t
{
    K1_PIN      = 17,
    K2_PIN      = 18,
    K3_PIN      = 22,
    K4_PIN      = 23,
    R_PIN       = 20,
    L_PIN       = 21,
    UP_PIN      = 13,
    LEFT_PIN    = 5,
    DOWN_PIN    = 26,
    RIGHT_PIN   = 19,
    CENTER_PIN  = 6
};

enum class OutputKey : uint32_t
{
    PIN_RST  = 27,   
    PIN_DC   = 25,   
    PIN_BL   = 24
};

class Gpio
{
private:
    int fd = -1;
public:
    Gpio(const std::string& device, int flags);
    ~Gpio();

    Gpio(const Gpio&)               = delete;
    Gpio& operator=(const Gpio&)    = delete;

    Gpio(const Gpio&&)              = delete;
    Gpio& operator=(const Gpio&&)   = delete;

    int get_fd() const { return fd; }
};

class GpioRead : public IRead
{
private:
    int fd = -1;
public:
    GpioRead(int gpio_fd);
    ~GpioRead();

    void read(Span<std::byte> buffer) override;
};

class GpioWrite : public IWrite
{
private:
    int fd = -1;
public:
    GpioWrite(int gpio_fd);
    ~GpioWrite();

    void write(Span<const std::byte> buffer) override;
}; 
 