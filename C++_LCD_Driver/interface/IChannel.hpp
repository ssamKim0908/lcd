#pragma once
#include "IFdReadable.hpp"
#include "../util/span.hpp"
#include <cstddef>

struct RecvResult;

enum class SendStatus { Ok, Closed };

class ISender
{
public:
    virtual ~ISender() = default;
    virtual SendStatus send(util::Span<const std::byte> data) = 0;
};

class IReceiver
{
public:
    virtual ~IReceiver() = default;
    virtual RecvResult recv() = 0;
};

class IChannel : public ISender, public IReceiver, public IFdReadable
{
public:
    ~IChannel() override = default;
};
