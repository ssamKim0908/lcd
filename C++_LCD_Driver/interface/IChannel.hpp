#pragma once
#include "IFdReadable.hpp"
#include "../util/span.hpp"
#include <cstddef>

struct RecvResult;

enum class SendStatus { Ok, Closed };

class IChannel : public IFdReadable
{
public:
    virtual SendStatus send(util::Span<const std::byte> data) = 0;
    virtual RecvResult recv()                                 = 0;
    ~IChannel() override = default;
};
