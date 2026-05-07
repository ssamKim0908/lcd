#pragma once
#include "../interface/IRenderCommand.hpp"
#include <memory>

class Rasterizer;
struct Packet;

class CommandFactory
{
private:
    std::shared_ptr<Rasterizer> receiver;
public:
    explicit CommandFactory(std::shared_ptr<Rasterizer> receiver);
    ~CommandFactory();

    CommandFactory(const CommandFactory&)            = delete;
    CommandFactory& operator=(const CommandFactory&) = delete;

    std::unique_ptr<IRenderCommand> create(const Packet& packet);
};
