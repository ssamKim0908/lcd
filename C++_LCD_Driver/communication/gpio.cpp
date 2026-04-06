#include "gpio.hpp"
#include "pch.h"

Fd_Gpio::Fd_Gpio(const std::string& device, int flags) : fd_t(device, flags) {}

void Fd_Gpio::read(span<uint8_t> buffer)
{
    std::cout << "Reading from GPIO device with file descriptor: " << fd_t.get_fd() << std::endl;
}