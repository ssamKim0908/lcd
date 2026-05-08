#pragma once
#include "../sdk/KeyEvent.hpp"

class IKeys
{
public:
    virtual KeyEvent next_event() = 0;
    virtual ~IKeys() = default;
};
