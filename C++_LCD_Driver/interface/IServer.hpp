#pragma once
#include "IFdReadable.hpp"
#include <memory>

class IChannel;

class IServer : public IFdReadable
{
public:
    virtual std::unique_ptr<IChannel> accept() = 0;
    ~IServer() override = default;
};
