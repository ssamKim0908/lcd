#include "spi.hpp"
#include <linux/spi/spidev.h>
#include "pch.h"

void FdSpi::write(const span<uint8_t> buffer)
{
    std::cout << "Writing to SPI device with file descriptor: " << FdT.get_fd() << std::endl;
    //::write(FdT.get_fd(), buffer.data(), buffer.size());
}