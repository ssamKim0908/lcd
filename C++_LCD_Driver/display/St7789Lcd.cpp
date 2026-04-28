#include "St7789Lcd.hpp"
#include "LcdWriter.hpp"
#include "../include/span.hpp"
#include "../util/time.hpp"
#include <vector>
#include <initializer_list>

St7789Lcd::St7789Lcd(std::unique_ptr<LcdWriter> writer)
    : writer(std::move(writer))
{}

St7789Lcd::~St7789Lcd() = default;

void St7789Lcd::init()
{
    using namespace util::time;

    writer->reset();

    auto cmd  = [this](uint8_t b) { writer->write_cmd(std::byte{b}); };
    auto data = [this](std::initializer_list<uint8_t> bs) {
        std::vector<std::byte> buf;
        buf.reserve(bs.size());
        for (uint8_t b : bs) buf.push_back(std::byte{b});
        writer->write_data({buf.data(), buf.size()});
    };

    cmd(0x11);                                          // sleep out
    sleep_ms(1000);

    cmd(0x36); data({0x60});                            // MADCTL: MY=0,MX=1,MV=1,ML=0,RGB
    sleep_ms(50);
    cmd(0x3A); data({0x05});                            // 65k colors

    cmd(0xB2); data({0x0C, 0x0C, 0x00, 0x33, 0x33});    // PORCTRL
    cmd(0xB7); data({0x35});                            // GCTRL
    cmd(0xBB); data({0x37});                            // VCOMS
    cmd(0xC0); data({0x2C});                            // LCMCTRL
    cmd(0xC2); data({0x01});                            // VDVVRHEN
    cmd(0xC3); data({0x12});                            // VRHS
    cmd(0xC4); data({0x20});                            // VDVS
    cmd(0xC6); data({0x0F});                            // FRCTRL2 (60Hz)
    cmd(0xD0); data({0xA4, 0xA1});                      // PWCTRL1

    cmd(0xE0); data({0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F,
                     0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23});
    cmd(0xE1); data({0xD4, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F,
                     0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23});

    cmd(0x21);                                          // INVON
    sleep_ms(120);
    cmd(0x29);                                          // DISPON
    sleep_ms(50);
}

void St7789Lcd::set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    x1 -= 1;
    y1 -= 2;

    writer->write_cmd(std::byte{0x2A});
    std::byte col[] = {
        std::byte((x0 >> 8) & 0xFF), std::byte(x0 & 0xFF),
        std::byte((x1 >> 8) & 0xFF), std::byte(x1 & 0xFF),
    };
    writer->write_data({col, 4});

    writer->write_cmd(std::byte{0x2B});
    std::byte row[] = {
        std::byte((y0 >> 8) & 0xFF), std::byte(y0 & 0xFF),
        std::byte((y1 >> 8) & 0xFF), std::byte(y1 & 0xFF),
    };
    writer->write_data({row, 4});

    writer->write_cmd(std::byte{0x2C});
}

void St7789Lcd::clear(uint16_t color)
{
    uint16_t be = static_cast<uint16_t>((color >> 8) | (color << 8));

    std::vector<uint16_t> row(WIDTH, be);
    Span<const std::byte> row_bytes(row);

    set_window(0, 0, WIDTH, HEIGHT);

    for (uint16_t y = 0; y < HEIGHT; ++y)
    {
        writer->write_data(row_bytes);
    }
}

void St7789Lcd::draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    writer->write_cmd(std::byte{0x2A});
    std::byte col[] = {
        std::byte((x >> 8) & 0xFF), std::byte(x & 0xFF),
        std::byte((x >> 8) & 0xFF), std::byte(x & 0xFF),
    };
    writer->write_data({col, 4});

    writer->write_cmd(std::byte{0x2B});
    std::byte row[] = {
        std::byte((y >> 8) & 0xFF), std::byte(y & 0xFF),
        std::byte((y >> 8) & 0xFF), std::byte(y & 0xFF),
    };
    writer->write_data({row, 4});

    writer->write_cmd(std::byte{0x2C});

    std::byte px[] = {
        std::byte((color >> 8) & 0xFF),
        std::byte(color & 0xFF),
    };
    writer->write_data({px, 2});
}
