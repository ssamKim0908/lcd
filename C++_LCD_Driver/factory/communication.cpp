#include "communication.hpp"
#include "../communication/spi.hpp"
#include "../communication/gpio.hpp"

std::unique_ptr<IRead> LcdFactoryCommunication::create_read_interface()
{
    std::cout << "Creating GPIO read interface for /dev/gpiochip0" << std::endl;
    return nullptr;
    return std::make_unique<FdGpio>("/dev/gpiochip0", O_RDONLY);
}

std::unique_ptr<IWrite> LcdFactoryCommunication::create_write_interface()
{
    std::cout << "Creating SPI write interface for /dev/spidev0.0" << std::endl;
    return nullptr;
    return std::make_unique<FdSpi>("/dev/spidev0.0", O_WRONLY);
}