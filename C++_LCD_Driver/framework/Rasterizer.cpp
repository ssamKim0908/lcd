#include "Rasterizer.hpp"
#include "../interface/ILcd.hpp"

Rasterizer::Rasterizer(std::unique_ptr<ILcd> lcd) : lcd(std::move(lcd)) {}
Rasterizer::~Rasterizer() = default;

void Rasterizer::clear(uint16_t color)
{
    lcd->clear(color);
}

void Rasterizer::fill_rect(int x, int y, int w, int h, uint16_t color)
{
    for (int j = y; j < y + h; ++j)
        for (int i = x; i < x + w; ++i)
            lcd->draw_pixel(static_cast<uint16_t>(i), static_cast<uint16_t>(j), color);
}

void Rasterizer::draw_rect(int x, int y, int w, int h, uint16_t color)
{
    for (int i = x; i < x + w; ++i)
    {
        lcd->draw_pixel(static_cast<uint16_t>(i), static_cast<uint16_t>(y),         color);
        lcd->draw_pixel(static_cast<uint16_t>(i), static_cast<uint16_t>(y + h - 1), color);
    }
    for (int j = y; j < y + h; ++j)
    {
        lcd->draw_pixel(static_cast<uint16_t>(x),         static_cast<uint16_t>(j), color);
        lcd->draw_pixel(static_cast<uint16_t>(x + w - 1), static_cast<uint16_t>(j), color);
    }
}

void Rasterizer::draw_circle(int cx, int cy, int r, uint16_t color)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    auto plot8 = [&](int dx, int dy) {
        lcd->draw_pixel(static_cast<uint16_t>(cx + dx), static_cast<uint16_t>(cy + dy), color);
        lcd->draw_pixel(static_cast<uint16_t>(cx - dx), static_cast<uint16_t>(cy + dy), color);
        lcd->draw_pixel(static_cast<uint16_t>(cx + dx), static_cast<uint16_t>(cy - dy), color);
        lcd->draw_pixel(static_cast<uint16_t>(cx - dx), static_cast<uint16_t>(cy - dy), color);
        lcd->draw_pixel(static_cast<uint16_t>(cx + dy), static_cast<uint16_t>(cy + dx), color);
        lcd->draw_pixel(static_cast<uint16_t>(cx - dy), static_cast<uint16_t>(cy + dx), color);
        lcd->draw_pixel(static_cast<uint16_t>(cx + dy), static_cast<uint16_t>(cy - dx), color);
        lcd->draw_pixel(static_cast<uint16_t>(cx - dy), static_cast<uint16_t>(cy - dx), color);
    };

    plot8(x, y);
    while (y >= x)
    {
        ++x;
        if (d > 0) { --y; d += 4 * (x - y) + 10; }
        else       {      d += 4 *  x      +  6; }
        plot8(x, y);
    }
}

void Rasterizer::fill_circle(int cx, int cy, int r, uint16_t color)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    auto hline = [&](int from_x, int to_x, int yy) {
        for (int i = from_x; i <= to_x; ++i)
            lcd->draw_pixel(static_cast<uint16_t>(i), static_cast<uint16_t>(yy), color);
    };

    auto fill = [&](int dx, int dy) {
        hline(cx - dx, cx + dx, cy + dy);
        hline(cx - dx, cx + dx, cy - dy);
        hline(cx - dy, cx + dy, cy + dx);
        hline(cx - dy, cx + dy, cy - dx);
    };

    fill(x, y);
    while (y >= x)
    {
        ++x;
        if (d > 0) { --y; d += 4 * (x - y) + 10; }
        else       {      d += 4 *  x      +  6; }
        fill(x, y);
    }
}

void Rasterizer::draw_text(int x, int y, const std::string& text, util::font::TextSize size, uint16_t color)
{
    using namespace util::font;

    const Font& font           = get_font(size);
    const int   bytes_per_row  = (font.width + 7) / 8;
    const int   bytes_per_char = bytes_per_row * font.height;

    int cur_x = x;
    for (char c : text)
    {
        if (c < 0x20 || c > 0x7E)
        {
            cur_x += font.width;
            continue;
        }

        const size_t base = static_cast<size_t>(c - 0x20) * bytes_per_char;

        for (int row = 0; row < font.height; ++row)
        {
            for (int col = 0; col < font.width; ++col)
            {
                const size_t byte_idx = base + row * bytes_per_row + col / 8;
                const int    bit_idx  = 7 - (col % 8);
                if (font.data[byte_idx] & (1 << bit_idx))
                {
                    lcd->draw_pixel(
                        static_cast<uint16_t>(cur_x + col),
                        static_cast<uint16_t>(y + row),
                        color);
                }
            }
        }
        cur_x += font.width;
    }
}

void Rasterizer::render()
{
    lcd->render();
}
