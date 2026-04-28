#pragma once
#include "../include/common.h"
#include "../include/span.hpp"
#include <cstddef>

class Spi;
class GpioWrite;

class LcdWriter
{
private:
    std::unique_ptr<Spi>       spi;
    std::unique_ptr<GpioWrite> gpio;
public:
    LcdWriter(std::unique_ptr<Spi> spi, std::unique_ptr<GpioWrite> gpio);
    ~LcdWriter();

    LcdWriter(const LcdWriter&)            = delete;
    LcdWriter& operator=(const LcdWriter&) = delete;

    void reset      ();
    void write_cmd  (std::byte cmd);
    void write_data (Span<const std::byte> data);
};
