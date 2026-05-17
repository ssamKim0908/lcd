#pragma once
#include "../interface/IProxyClientToServer.hpp"
#include "../util/Font.hpp"
#include <memory>
#include <string>
#include <cstdint>

class ILcd;

class Rasterizer : public IProxyClientToServer
{
private:
    std::unique_ptr<ILcd> lcd;
public:
    explicit Rasterizer(std::unique_ptr<ILcd> lcd);
    ~Rasterizer() override;

    Rasterizer(const Rasterizer&)            = delete;
    Rasterizer& operator=(const Rasterizer&) = delete;

    void clear      (uint16_t color) override;
    void fill_rect  (int x,  int y,  int w, int h, uint16_t color) override;
    void draw_rect  (int x,  int y,  int w, int h, uint16_t color) override;
    void fill_circle(int cx, int cy, int r,        uint16_t color) override;
    void draw_circle(int cx, int cy, int r,        uint16_t color) override;
    void draw_text  (int x,  int y,  const std::string& text, util::font::TextSize size, uint16_t color) override;
    void render     () override;
};
