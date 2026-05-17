#pragma once
#include "../interface/ICommand.hpp"
#include "../interface/IProxyServer.hpp"
#include <memory>

class Rasterizer;
struct Packet;

class SimpleCommandFactory : public IProxyServerFromClient
{
private:
    std::shared_ptr<Rasterizer> receiver;
public:
    explicit SimpleCommandFactory(std::shared_ptr<Rasterizer> receiver);
    ~SimpleCommandFactory();

    SimpleCommandFactory(const SimpleCommandFactory&)            = delete;
    SimpleCommandFactory& operator=(const SimpleCommandFactory&) = delete;

    std::unique_ptr<ICommand> create(const Packet& packet);
};
