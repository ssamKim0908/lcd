#include "App.hpp"
#include "Draw.hpp"
#include "internal/IpcClient.hpp"
#include "../interface/IChannel.hpp"
#include "../ipc/UdsClient.hpp"
#include "../shared/Paths.hpp"

namespace sdk
{

struct App::Impl
{
    std::shared_ptr<internal::IpcClient> ipc;
    std::unique_ptr<Draw>                draw;
};

App::App()
    : impl_(std::make_unique<Impl>())
{
    auto channel = UdsClient(shared::SOCK_PATH).connect();
    impl_->ipc   = std::make_shared<internal::IpcClient>(std::move(channel));
    impl_->draw  = std::make_unique<Draw>(impl_->ipc);
}

App::~App() = default;

Draw& App::draw()
{
    return *impl_->draw;
}

void App::run()
{
    on_render();
    while (true)
    {
        KeyEvent ev = impl_->ipc->recv_key();
        on_key(ev);
        on_render();
    }
}

}
