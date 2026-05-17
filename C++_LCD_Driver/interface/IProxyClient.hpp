#pragma once
#include <cstdint>
#include <string>
#include "../util/Font.hpp"

/**
 * @brief IDrawContract defines the common drawing interface.
 * Aligned with Rasterizer's parameter ordering and types.
 */
class IProxyClientToServer
{
public:
    virtual ~IProxyClientToServer() = default;

    virtual void clear      (uint16_t color) = 0;
    virtual void fill_rect  (int x, int y, int w, int h, uint16_t color) = 0;
    virtual void draw_rect  (int x, int y, int w, int h, uint16_t color) = 0;
    virtual void fill_circle(int cx, int cy, int r, uint16_t color) = 0;
    virtual void draw_circle(int cx, int cy, int r, uint16_t color) = 0;
    virtual void draw_text  (int x, int y, const std::string& text, util::font::TextSize size, uint16_t color) = 0;
    virtual void render     () = 0;
};
