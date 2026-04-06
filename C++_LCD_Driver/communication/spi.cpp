#include "spi.hpp"
#include <linux/spi/spidev.h>
#include "pch.h"

void Fd_Spi::write(const span<uint8_t> buffer)
{
    std::cout << "Writing to SPI device with file descriptor: " << fd_t.get_fd() << std::endl;
    ::write(fd_t.get_fd(), buffer.data(), buffer.size());
}