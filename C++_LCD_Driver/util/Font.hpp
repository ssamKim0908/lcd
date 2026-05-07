#pragma once
#include "span.hpp"
#include <cstdint>

namespace util::font
{
    enum class TextSize : uint8_t
    {
        Small,
    };

    struct Font
    {
        uint8_t             width;
        uint8_t             height;
        Span<const uint8_t> data;
    };

    const Font& get_font(TextSize size);
}
