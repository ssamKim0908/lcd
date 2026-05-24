#pragma once
#include "../../util/Font.hpp"
#include <cstdint>
#include <memory>
#include <string>

class IProxyClientToServer;

namespace sdk
{

class DrawImpl
{
public:
    explicit DrawImpl(std::shared_ptr<IProxyClientToServer> proxy);
    ~DrawImpl();

    DrawImpl(const DrawImpl&)            = delete;
    DrawImpl& operator=(const DrawImpl&) = delete;

    void clear      (uint16_t color);
    void fill_rect  (int x, int y, int w, int h, uint16_t color);
    void draw_rect  (int x, int y, int w, int h, uint16_t color);
    void fill_circle(int cx, int cy, int r, uint16_t color);
    void draw_circle(int cx, int cy, int r, uint16_t color);
    void draw_text  (int x, int y, const std::string& text, util::font::TextSize size,
                     uint16_t color);

private:
    std::shared_ptr<IProxyClientToServer> proxy_;
};

}
