#include "KeyImpl.hpp"
#include "ipc/IpcCFromS.hpp"

#include <utility>

namespace sdk
{

KeyImpl::KeyImpl(std::unique_ptr<IProxyClientFromServer> proxy)
    : proxy_(std::move(proxy)) {}

KeyImpl::~KeyImpl() = default;

KeyEvent KeyImpl::next()
{
    return proxy_->recv_key();
}

}
