#include "spi.hpp"
#include <linux/spi/spidev.h>

FdSpi::FdSpi(const std::string& device, int flags) : FdT(device, flags)
{
    int fd = FdT.get_fd();

    mode  = SPI_MODE_0;
    bits  = 8;
    speed = 1000000;
    lsb   = 0;

    ioctl(fd, SPI_IOC_WR_MODE,          &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ,  &speed);
    ioctl(fd, SPI_IOC_WR_LSB_FIRST,     &lsb);
}

void FdSpi::write(span<const std::byte> buffer)
{
    std::cout << "Writing to SPI device with file descriptor: " << FdT.get_fd() << std::endl;
    //::write(FdT.get_fd(), buffer.data(), buffer.size());
}