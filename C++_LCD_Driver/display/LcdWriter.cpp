#include "LcdWriter.hpp"
#include "../hal/spi.hpp"
#include "../hal/gpio.hpp"
#include "../util/time.hpp"

LcdWriter::LcdWriter(std::unique_ptr<Spi> spi, std::unique_ptr<GpioWrite> gpio)
    : spi(std::move(spi)), gpio(std::move(gpio))
{}

LcdWriter::~LcdWriter() = default;

void LcdWriter::reset()
{
    using namespace util::time;
    sleep_ms(100);
    gpio->write_pin(OutputKey::PIN_RST, GpioValue::Low);
    sleep_ms(100);
    gpio->write_pin(OutputKey::PIN_RST, GpioValue::High);
    sleep_ms(100);
}

void LcdWriter::write_cmd(std::byte cmd)
{
    gpio->write_cmd();
    spi->write({&cmd, 1});
}

void LcdWriter::write_data(Span<const std::byte> data)
{
    gpio->write_data();
    spi->write(data);
}
