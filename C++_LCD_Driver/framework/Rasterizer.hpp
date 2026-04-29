#pragma once
#include "../util/Font.hpp"
#include <memory>
#include <string>
#include <cstdint>

class ILcd;

class Rasterizer
{
private:
    std::unique_ptr<ILcd> lcd;
public:
    explicit Rasterizer(std::unique_ptr<ILcd> lcd);
    ~Rasterizer();

    Rasterizer(const Rasterizer&)            = delete;
    Rasterizer& operator=(const Rasterizer&) = delete;

    void clear      (uint16_t color);
    void fill_rect  (int x,  int y,  int w, int h, uint16_t color);
    void draw_rect  (int x,  int y,  int w, int h, uint16_t color);
    void fill_circle(int cx, int cy, int r,        uint16_t color);
    void draw_circle(int cx, int cy, int r,        uint16_t color);
    void draw_text  (int x,  int y,  const std::string& text, util::font::TextSize size, uint16_t color);
    void render     ();
};
