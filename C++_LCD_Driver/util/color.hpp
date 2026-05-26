#pragma once
#include <cstdint>

namespace util::color
{
    using Rgb565 = uint16_t;

    constexpr Rgb565 BLACK      = 0x0000;
    constexpr Rgb565 WHITE      = 0xFFFF;
    constexpr Rgb565 RED        = 0xF800;
    constexpr Rgb565 GREEN      = 0x07E0;
    constexpr Rgb565 BLUE       = 0x001F;
    constexpr Rgb565 YELLOW     = 0xFFE0;
    constexpr Rgb565 CYAN       = 0x07FF;
    constexpr Rgb565 MAGENTA    = 0xF81F;

    constexpr Rgb565 LIGHT_GRAY = 0xC618;
    constexpr Rgb565 GRAY       = 0x7BEF;
    constexpr Rgb565 DARK_GRAY  = 0x4208;

    constexpr Rgb565 DARK_GREEN = 0x0500;
    constexpr Rgb565 NAVY       = 0x0010;
    constexpr Rgb565 MAROON     = 0x8000;
}
