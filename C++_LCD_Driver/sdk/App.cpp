#include "App.hpp"
#include "Key.hpp"
#include "internal/AppImpl.hpp"

namespace sdk
{

App::App()
    : impl_(std::make_unique<AppImpl>()) {}

App::~App() = default;

Draw& App::draw() { return impl_->draw(); }
Key&  App::key()  { return impl_->key();  }

void App::exit() { running_ = false; }

void App::run()
{
    running_ = true;
    on_render();
    while (running_)
    {
        KeyEvent ev = key().get_key();
        on_key(ev);
        if (!running_) break;
        on_render();
    }
}

}
