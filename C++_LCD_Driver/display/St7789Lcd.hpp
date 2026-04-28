#pragma once
#include "../interface/ILcd.hpp"
#include "../include/common.h"

class LcdWriter;

class St7789Lcd : public ILcd
{
public:
    static constexpr uint16_t WIDTH  = 240;
    static constexpr uint16_t HEIGHT = 240;

private:
    std::unique_ptr<LcdWriter> writer;

    void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

public:
    St7789Lcd(std::unique_ptr<LcdWriter> writer);
    ~St7789Lcd() override;

    St7789Lcd(const St7789Lcd&)            = delete;
    St7789Lcd& operator=(const St7789Lcd&) = delete;

    void init       ()                                       override;
    void clear      (uint16_t color)                         override;
    void draw_pixel (uint16_t x, uint16_t y, uint16_t color) override;
};
