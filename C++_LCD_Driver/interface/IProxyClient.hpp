#pragma once
#include "../shared/KeyEvent.hpp"
#include "../util/Font.hpp"
#include <cstdint>
#include <string>

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

class IProxyClientFromServer
{
public:
    virtual ~IProxyClientFromServer() = default;
    virtual KeyEvent recv_key() = 0;
};
