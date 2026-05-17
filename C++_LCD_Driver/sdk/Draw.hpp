#pragma once
#include <cstdint>
#include <memory>
#include <string>

namespace sdk
{

class Draw
{
public:
    Draw();
    ~Draw() = default;

    Draw(const Draw&)            = delete;
    Draw& operator=(const Draw&) = delete;

    void clear      (uint16_t color);
    void fill_rect  (int x, int y, int w, int h, uint16_t color);
    void draw_rect  (int x, int y, int w, int h, uint16_t color);
    void fill_circle(int cx, int cy, int r, uint16_t color);
    void draw_circle(int cx, int cy, int r, uint16_t color);
    void draw_text  (int x, int y, const std::string& text, util::font::TextSize size,
                     uint16_t color);
    void render     ();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}
