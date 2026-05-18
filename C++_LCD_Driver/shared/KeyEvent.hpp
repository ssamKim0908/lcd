#pragma once
#include <cstdint>

enum class Key : uint8_t
{
    K1, K2, K3, K4,
    R, L,
    Up, Down, Left, Right, Center
};

enum class KeyState : uint8_t
{
    Pressed,
    Released
};

struct KeyEvent
{
    Key      key;
    KeyState state;
};
