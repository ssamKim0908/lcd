#pragma once
#include <memory>

class IChannel;

class IServer
{
public:
    virtual std::unique_ptr<IChannel> accept() = 0;
    virtual ~IServer() = default;
};
