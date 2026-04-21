#pragma once

#include "../include/common.h"
#include "../include/span.hpp"

class IRead
{
public:
    virtual void read(span<std::byte> buffer) = 0;
    virtual ~IRead() = default;
};

class IWrite
{
public:
    virtual void write(span<const std::byte> buffer) = 0;
    virtual ~IWrite() = default;
};