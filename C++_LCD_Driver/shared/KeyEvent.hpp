#pragma once

enum class Key
{
    K1, K2, K3, K4,
    R, L,
    Up, Down, Left, Right, Center
};

enum class KeyState
{
    Pressed,
    Released
};

struct KeyEvent
{
    Key      key;
    KeyState state;
};
