#include "transport_interface.hpp"
#include "transport_gpio.hpp"
#include "pch.h"

void Fd_Gpio::read(span<uint8_t> buffer)
{
    std::cout << "Reading from GPIO device with file descriptor: " << fd_t.get_fd() << std::endl;
}

void Fd_Gpio::ioctl(uint32_t flag, void* arg)
{
    std::cout << "Performing ioctl on GPIO device with file descriptor: " << fd_t.get_fd() << std::endl;
}