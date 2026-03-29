#include "transport_interface.hpp"
#include "transport_gpio.hpp"
#include "pch.h"

void Fd_gpio::read()
{
    std::cout << "Reading from GPIO device with file descriptor: " << fd << std::endl;
}

void Fd_gpio::ioctl()
{
    std::cout << "Performing ioctl on GPIO device with file descriptor: " << fd << std::endl;
}