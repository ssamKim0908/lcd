#include "communication.hpp"
#include "../service/LcdIO.hpp"
#include "../hal/gpio.hpp"

LcdFactoryCommunication::LcdFactoryCommunication()
{
#ifdef TARGET_DEVICE
    gpio = std::make_unique<Gpio>("/dev/gpiochip0", O_RDWR);
#endif
}

LcdFactoryCommunication::~LcdFactoryCommunication() = default;

int LcdFactoryCommunication::get_gpio_fd()
{
    return gpio->get_fd();
}

std::unique_ptr<IRead> LcdFactoryCommunication::create_read_interface()
{
    std::cout << "Creating GPIO read interface for /dev/gpiochip0" << std::endl;
#ifdef TARGET_DEVICE
    return std::make_unique<LcdReadService>(std::make_unique<GpioRead>(get_gpio_fd()));
#else
    return nullptr;
#endif
}

std::unique_ptr<IWrite> LcdFactoryCommunication::create_write_interface()
{
    std::cout << "Creating SPI write interface for /dev/spidev0.0" << std::endl;
#ifdef TARGET_DEVICE
    auto spi = std::make_unique<Spi>("/dev/spidev0.0", O_WRONLY);
    auto gpio_write = std::make_unique<GpioWrite>(get_gpio_fd());
    return std::make_unique<LcdWriteService>(std::move(spi), std::move(gpio_write));
#else
    return nullptr;
#endif
}