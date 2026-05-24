#include "AppImpl.hpp"
#include "DrawImpl.hpp"
#include "KeyImpl.hpp"
#include "RenderImpl.hpp"
#include "ipc/IpcCToS.hpp"
#include "ipc/IpcCFromS.hpp"
#include "../Draw.hpp"
#include "../Key.hpp"
#include "../../interface/IChannel.hpp"
#include "../../interface/IProxyClient.hpp"
#include "../../osal/ipc/UdsClient.hpp"
#include "../../shared/SocketPaths.hpp"

#include <utility>

namespace sdk
{

AppImpl::AppImpl()
{
    std::shared_ptr<IChannel> channel = UdsClient(shared::SOCK_PATH).connect();

    std::shared_ptr<IProxyClientToServer> cToS =
        std::make_shared<internal::IpcCToS>(channel);
    auto cFromS = std::make_unique<internal::IpcCFromS>(channel);

    auto drawImpl   = std::make_unique<DrawImpl>  (cToS);
    auto renderImpl = std::make_unique<RenderImpl>(std::move(cToS));
    auto keyImpl    = std::make_unique<KeyImpl>   (std::move(cFromS));

    draw_        = std::make_unique<Draw>(std::move(drawImpl));
    key_         = std::make_unique<Key> (std::move(keyImpl));
    render_impl_ = std::move(renderImpl);
}

AppImpl::~AppImpl() = default;

Draw&       AppImpl::draw()        { return *draw_; }
Key&        AppImpl::key()         { return *key_;  }
void        AppImpl::render()      { render_impl_->render(); }
}
