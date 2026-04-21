#include "communication.hpp"
#include "../hal/spi.hpp"
#include "../hal/gpio.hpp"

std::unique_ptr<IRead> LcdFactoryCommunication::create_read_interface()
{
    std::cout << "Creating GPIO read interface for /dev/gpiochip0" << std::endl;
#ifdef TARGET_DEVICE
    return std::make_unique<gpio>("/dev/gpiochip0", O_RDONLY);
#else
    return nullptr;
#endif
}

std::unique_ptr<IWrite> LcdFactoryCommunication::create_write_interface()
{
    std::cout << "Creating SPI write interface for /dev/spidev0.0" << std::endl;
#ifdef TARGET_DEVICE
    return std::make_unique<spi>("/dev/spidev0.0", O_WRONLY);
#else
    return nullptr;
#endif
}