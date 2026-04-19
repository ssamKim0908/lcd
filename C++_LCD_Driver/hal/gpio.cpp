#include "gpio.hpp"

FdGpio::FdGpio(const std::string& device, int flags) : FdT(device, flags) {}

void FdGpio::read(span<std::byte> buffer)
{
#ifdef TARGET_DEVICE
    ::read(FdT.get_fd(), buffer.data(), buffer.size());
#else
    std::cout << "Reading from GPIO device with file descriptor: " << FdT.get_fd() << std::endl;
#endif
}