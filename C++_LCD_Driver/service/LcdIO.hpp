#pragma once
#include "../interface/ICommunication.hpp"

class Spi;
class Gpio;
class GpioRead;
class GpioWrite;

class LcdReadService : public IRead
{
private:
    std::unique_ptr<GpioRead> gpio_read;
private:
    bool init();
public:
    LcdReadService(std::unique_ptr<GpioRead> gpio_read);
    void read(Span<std::byte> buffer) override;
};

class LcdWriteService : public IWrite
{
private:
    std::unique_ptr<Spi> spi;
    std::unique_ptr<GpioWrite> gpio_write;
private:
    bool init();
    void lcd_reset();
    void spi_write_cmd  (Span<const std::byte> cmd);
    void spi_write_data (Span<const std::byte> buffer);
public:
    LcdWriteService(std::unique_ptr<Spi> spi, std::unique_ptr<GpioWrite> gpio_write);
    void write(Span<const std::byte> buffer) override;
};
