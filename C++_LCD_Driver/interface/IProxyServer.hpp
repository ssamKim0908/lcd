#pragma once
#include <memory>

class ICommand;
struct Packet;

/**
 * @brief IDrawContract defines the common drawing interface.
 * Aligned with Rasterizer's parameter ordering and types.
 */
class IProxyServerFromClient
{
public:
    virtual ~IProxyServerFromClient() = default;

    virtual std::unique_ptr<ICommand> create(const Packet& packet) = 0;
};
