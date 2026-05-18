#pragma once
#include "IChannel.hpp"
#include "../shared/KeyEvent.hpp"
#include <memory>

class ICommand;
struct Packet;

class IProxyServerFromClient
{
public:
    virtual ~IProxyServerFromClient() = default;
    virtual std::unique_ptr<ICommand> create(const Packet& packet) = 0;
};

class IProxyServerToClient
{
public:
    virtual ~IProxyServerToClient() = default;
    virtual SendStatus on_key(KeyEvent ev) = 0;
};
