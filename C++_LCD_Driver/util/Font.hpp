#pragma once
#include "span.hpp"
#include "../sdk/TextSize.hpp"
#include <cstdint>

namespace util::font
{
    struct Font
    {
        uint8_t             width;
        uint8_t             height;
        Span<const uint8_t> data;
    };

    const Font& get_font(TextSize size);
}
