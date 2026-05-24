#include "RenderImpl.hpp"
#include "../../interface/IProxyClient.hpp"

#include <utility>

namespace sdk
{

RenderImpl::RenderImpl(std::shared_ptr<IProxyClientToServer> proxy)
    : proxy_(std::move(proxy)) {}

RenderImpl::~RenderImpl() = default;

void RenderImpl::render()
{
    proxy_->render();
}

}
