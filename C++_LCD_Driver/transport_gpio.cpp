#include "transport_interface.hpp"
#include "transport_gpio.hpp"
#include "pch.h"

void Fd_gpio::Read(uint8_t* buffer, uint32_t length)
{
    std::cout << "Reading from GPIO device with file descriptor: " << fd << std::endl;
}

void Fd_gpio::Ioctl(uint32_t flag, void* arg)
{
    std::cout << "Performing ioctl on GPIO device with file descriptor: " << fd << std::endl;
}