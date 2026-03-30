#include "transport_interface.hpp"
#include "transport_spi.hpp"
#include <linux/spi/spidev.h>
#include "pch.h"

void Fd_Spi::Write(const uint8_t* buffer, uint32_t length)
{
    std::cout << "Writing to SPI device with file descriptor: " << fd_t.get_fd() << std::endl;
    write(fd_t.get_fd(), buffer, length);
}

void Fd_Spi::Ioctl(uint32_t flag, void* arg)
{
    std::cout << "Performing ioctl on SPI device with file descriptor: " << fd_t.get_fd() << std::endl;
    ioctl(fd_t.get_fd(), flag, arg);
}