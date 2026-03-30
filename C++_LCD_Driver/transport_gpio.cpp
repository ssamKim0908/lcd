#include "transport_interface.hpp"
#include "transport_gpio.hpp"
#include "pch.h"

void Fd_Gpio::Read(uint8_t* buffer, uint32_t length)
{
    std::cout << "Reading from GPIO device with file descriptor: " << fd_t.get_fd() << std::endl;
}

void Fd_Gpio::Ioctl(uint32_t flag, void* arg)
{
    std::cout << "Performing ioctl on GPIO device with file descriptor: " << fd_t.get_fd() << std::endl;
}