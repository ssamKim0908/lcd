#include "gpio.hpp"

gpio::gpio(const std::string& device, int flags)
{
#ifdef TARGET_DEVICE
    fd = ::open(device.c_str(), flags);
    if (fd < 0)
    {
        throw std::runtime_error("Failed to open GPIO device: " + device);
    }
    std::cout << "Opened GPIO fd: " << fd << std::endl;
#else
    (void)device;
    (void)flags;
#endif
}

gpio::~gpio()
{
    if (fd >= 0)
    {
        std::cout << "Closing GPIO fd: " << fd << std::endl;
        ::close(fd);
    }
}

void gpio::read(span<std::byte> buffer)
{
#ifdef TARGET_DEVICE
    ::read(fd, buffer.data(), buffer.size());
#else
    std::cout << "Reading from GPIO device with file descriptor: " << fd << std::endl;
#endif
}
