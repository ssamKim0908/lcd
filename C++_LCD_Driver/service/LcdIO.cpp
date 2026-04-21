#include "LcdIO.hpp"
#include "../hal/spi.hpp"
#include "../hal/gpio.hpp"

LcdReadService::LcdReadService(std::unique_ptr<GpioRead> gpio_read)
    : gpio_read(std::move(gpio_read))
{
}

void LcdReadService::read(Span<std::byte> buffer)
{
    gpio_read->read(buffer);
}

LcdWriteService::LcdWriteService(std::unique_ptr<Spi> spi, std::unique_ptr<GpioWrite> gpio_write)
    : spi(std::move(spi)), gpio_write(std::move(gpio_write))
{
}

void LcdWriteService::write(Span<const std::byte> buffer)
{
    spi->write(buffer);
}
