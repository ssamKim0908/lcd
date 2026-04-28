#pragma once

enum class Key
{
    K1, K2, K3, K4,
    R, L,
    Up, Down, Left, Right, Center
};

struct KeyEvent
{
    Key  key;
    bool pressed;
};

class IKeys
{
public:
    virtual KeyEvent next_event() = 0;
    virtual ~IKeys() = default;
};
