#pragma once
#include <cstdint>

namespace shared
{
    enum class DrawCommand : uint8_t
    {
        Clear      = 0,
        FillRect   = 1,
        DrawRect   = 2,
        FillCircle = 3,
        DrawCircle = 4,
        DrawText   = 5,
        Render     = 6,
    };
}
