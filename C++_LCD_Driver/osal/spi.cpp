#include "spi.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#ifdef TARGET_DEVICE
#include <linux/spi/spidev.h>
#include <cerrno>
#include <system_error>
#endif

Spi::Spi(const std::string& device, int flags)
{
#ifdef TARGET_DEVICE
    fd = ::open(device.c_str(), flags);
    if (fd < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "Failed to open SPI device: " + device);
    }
    std::cout << "Opened SPI fd: " << fd << std::endl;

    mode  = SPI_MODE_0;
    bits  = 8;
    speed = 32000000;
    lsb   = 0;

    auto test = [this](unsigned long request, void* arg, const std::string& name) {
        if (::ioctl(fd, request, arg) < 0)
        {
            throw std::system_error(errno, std::generic_category(),
                "SPI ioctl failed (" + name + ")");
        }
    };

    test(SPI_IOC_WR_MODE,          &mode,  "SPI_IOC_WR_MODE");
    test(SPI_IOC_WR_BITS_PER_WORD, &bits,  "SPI_IOC_WR_BITS_PER_WORD");
    test(SPI_IOC_WR_MAX_SPEED_HZ,  &speed, "SPI_IOC_WR_MAX_SPEED_HZ");
    test(SPI_IOC_WR_LSB_FIRST,     &lsb,   "SPI_IOC_WR_LSB_FIRST");
#else
    (void)device;
    (void)flags;
#endif
}

Spi::~Spi()
{
    if (fd >= 0)
    {
        std::cout << "Closing SPI fd: " << fd << std::endl;
        ::close(fd);
    }
}

void Spi::write(util::Span<const std::byte> buffer)
{
#ifdef TARGET_DEVICE
    ssize_t n = ::write(fd, buffer.data(), buffer.size());
    if (n < 0)
    {
        throw std::system_error(errno, std::generic_category(), "SPI write");
    }
    if (static_cast<size_t>(n) != buffer.size())
    {
        throw std::runtime_error("SPI partial write");
    }
#else
    std::cout << "Writing to SPI device with file descriptor: " << fd << std::endl;
#endif
}
