#include "transport_interface.hpp"
#include "transport_spi.hpp"
#include <linux/spi/spidev.h>
#include "pch.h"

void Fd_spi::write()
{
    std::cout << "Writing to SPI device with file descriptor: " << fd << std::endl;
}

void Fd_spi::ioctl()
{
    std::cout << "Performing ioctl on SPI device with file descriptor: " << fd << std::endl;
}