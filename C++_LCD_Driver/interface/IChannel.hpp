#pragma once
#include "../util/span.hpp"
#include <cstddef>

class IChannel
{
public:
    virtual void send(util::Span<const std::byte> data) = 0;
    virtual void recv(util::Span<std::byte>       data) = 0;
    virtual int  fd  () const                     = 0;
    virtual ~IChannel() = default;
};
