#include "spi.hpp"
#ifdef TARGET_DEVICE
#include <linux/spi/spidev.h>
#endif

spi::spi(const std::string& device, int flags)
{
#ifdef TARGET_DEVICE
    fd = ::open(device.c_str(), flags);
    if (fd < 0)
    {
        throw std::runtime_error("Failed to open SPI device: " + device);
    }
    std::cout << "Opened SPI fd: " << fd << std::endl;

    mode  = SPI_MODE_0;
    bits  = 8;
    speed = 1000000;
    lsb   = 0;

    ioctl(fd, SPI_IOC_WR_MODE,          &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ,  &speed);
    ioctl(fd, SPI_IOC_WR_LSB_FIRST,     &lsb);
#else
    (void)device;
    (void)flags;
#endif
}

spi::~spi()
{
    if (fd >= 0)
    {
        std::cout << "Closing SPI fd: " << fd << std::endl;
        ::close(fd);
    }
}

void spi::write(span<const std::byte> buffer)
{
#ifdef TARGET_DEVICE
    ::write(fd, buffer.data(), buffer.size());
#else
    std::cout << "Writing to SPI device with file descriptor: " << fd << std::endl;
#endif
}
