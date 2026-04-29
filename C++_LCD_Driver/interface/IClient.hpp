#pragma once
#include <memory>

class IChannel;

class IClient
{
public:
    virtual std::unique_ptr<IChannel> connect() = 0;
    virtual ~IClient() = default;
};
