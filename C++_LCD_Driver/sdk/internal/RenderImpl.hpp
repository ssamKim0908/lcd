#pragma once
#include <memory>

class IProxyClientToServer;

namespace sdk
{

class RenderImpl
{
public:
    explicit RenderImpl(std::shared_ptr<IProxyClientToServer> proxy);
    ~RenderImpl();

    RenderImpl(const RenderImpl&)            = delete;
    RenderImpl& operator=(const RenderImpl&) = delete;

    void render();

private:
    std::shared_ptr<IProxyClientToServer> proxy_;
};

}
