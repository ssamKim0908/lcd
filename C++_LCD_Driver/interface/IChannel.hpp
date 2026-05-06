#pragma once
#include "../include/span.hpp"
#include <cstddef>

class IChannel
{
public:
    virtual void send(Span<const std::byte> data) = 0;
    virtual void recv(Span<std::byte>       data) = 0;
    virtual int  fd  () const                     = 0;
    virtual ~IChannel() = default;
};
