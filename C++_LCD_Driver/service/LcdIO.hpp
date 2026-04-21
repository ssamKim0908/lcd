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
public:
    LcdReadService(std::unique_ptr<GpioRead> gpio_read);
    void read(Span<std::byte> buffer) override;
};

class LcdWriteService : public IWrite
{
private:
    std::unique_ptr<Spi> spi;
    std::unique_ptr<GpioWrite> gpio_write;
public:
    LcdWriteService(std::unique_ptr<Spi> spi, std::unique_ptr<GpioWrite> gpio_write);
    void write(Span<const std::byte> buffer) override;
};
