#pragma once
#include "IFdReadable.hpp"
#include "../shared/KeyEvent.hpp"

class IKeys : public IFdReadable
{
public:
    virtual KeyEvent next_event() = 0;
    ~IKeys() override = default;
};
