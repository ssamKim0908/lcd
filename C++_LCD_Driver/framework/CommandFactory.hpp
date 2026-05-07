#pragma once
#include "../interface/IRenderCommand.hpp"
#include <memory>

class Rasterizer;
struct Packet;

class SimpleCommandFactory
{
private:
    std::shared_ptr<Rasterizer> receiver;
public:
    explicit SimpleCommandFactory(std::shared_ptr<Rasterizer> receiver);
    ~SimpleCommandFactory();

    SimpleCommandFactory(const SimpleCommandFactory&)            = delete;
    SimpleCommandFactory& operator=(const SimpleCommandFactory&) = delete;

    std::unique_ptr<IRenderCommand> create(const Packet& packet);
};
