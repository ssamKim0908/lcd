#pragma once
#include <memory>

namespace sdk
{

class Draw;
class Key;
class RenderImpl;

class AppImpl
{
public:
    AppImpl();
    ~AppImpl();

    AppImpl(const AppImpl&)            = delete;
    AppImpl& operator=(const AppImpl&) = delete;

    Draw&       draw();
    Key&        key();

    void render();

private:
    std::unique_ptr<Draw>       draw_;
    std::unique_ptr<Key>        key_;
    std::unique_ptr<RenderImpl> render_impl_;
};

}
