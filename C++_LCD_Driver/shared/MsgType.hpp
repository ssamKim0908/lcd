#pragma once
#include <cstdint>

namespace shared
{
    enum class C2S : uint8_t
    {
        Clear      = 0,
        FillRect   = 1,
        DrawRect   = 2,
        FillCircle = 3,
        DrawCircle = 4,
        DrawText   = 5,
        Render     = 6,
    };

    enum class S2C : uint8_t
    {
        KeyEvent   = 0,
    };
}
