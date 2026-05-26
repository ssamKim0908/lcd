#pragma once
#include "span.hpp"
#include <cstdint>

namespace util::font
{
    struct Font
    {
        uint8_t             width;
        uint8_t             height;
        Span<const uint8_t> data;
    };

    enum class TextSize : uint8_t
    {
        Small,
    };

    int width (TextSize size);
    int height(TextSize size);

    const Font& get_font(TextSize size);
}
