#include "LcdIO.hpp"
#include "../hal/spi.hpp"
#include "../hal/gpio.hpp"
#include "../utils/time.hpp"

LcdReadService::LcdReadService(std::unique_ptr<GpioRead> gpio_read)
    : gpio_read(std::move(gpio_read))
{
    init();
}

bool LcdReadService::init()
{

    return true;
}

void LcdReadService::read(Span<std::byte> buffer)
{
    //gpio_read->read(buffer);
}

LcdWriteService::LcdWriteService(std::unique_ptr<Spi> spi, std::unique_ptr<GpioWrite> gpio_write)
    : spi(std::move(spi)), gpio_write(std::move(gpio_write))
{
    init();
}

bool LcdWriteService::init()
{
    using namespace utils::time;
    

    return true;
}

void LcdWriteService::lcd_reset()
{
    using namespace utils::time;
    sleep_ms(100);
    
    Lgpio_write_pin(PIN_RST, LGPIO_LOW);
    
    sleep_ms(100);
    Lgpio_write_pin(PIN_RST, LGPIO_HIGH);
    sleep_ms(100);
}

void LcdWriteService::spi_write_cmd(Span<const std::byte> cmd)
{
    gpio_write->write_cmd();
    spi->write(cmd);
}

void LcdWriteService::spi_write_data(Span<const std::byte> buffer)
{
    gpio_write->write_data();
    spi->write(buffer);
}

void LcdWriteService::write(Span<const std::byte> buffer)
{
    spi_write_data(buffer);
}
