#pragma once
#include "../util/span.hpp"
#include <cstddef>

struct Packet;

class IChannel
{
public:
    virtual void   send(util::Span<const std::byte> data) = 0;
    virtual Packet recv()                                 = 0;
    virtual int    fd  () const                           = 0;
    virtual ~IChannel() = default;
};
