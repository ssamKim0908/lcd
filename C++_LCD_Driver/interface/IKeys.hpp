#pragma once

enum class Key
{
    K1, K2, K3, K4,
    R, L,
    Up, Down, Left, Right, Center
};

class IKeys
{
public:
    virtual bool is_pressed(Key key) = 0;
    virtual ~IKeys() = default;
};
