#pragma once
#include <cstdint>

struct Cell
{
    bool    has_mine = false;
    bool    revealed = false;
    bool    flagged  = false;
    uint8_t adjacent = 0;
};
