#pragma once
#include <cstdint>

class ILcd
{
public:
    virtual void init       ()                                              = 0;
    virtual void clear      (uint16_t color)                                = 0;
    virtual void draw_pixel (uint16_t x, uint16_t y, uint16_t color)        = 0;
    virtual ~ILcd() = default;
};
