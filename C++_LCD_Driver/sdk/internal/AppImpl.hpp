#pragma once
#include <memory>

namespace sdk
{

class Draw;
class Key;

class AppImpl
{
public:
    AppImpl();
    ~AppImpl();

    AppImpl(const AppImpl&)            = delete;
    AppImpl& operator=(const AppImpl&) = delete;

    Draw& draw();
    Key&  key();

private:
    std::unique_ptr<Draw> draw_;
    std::unique_ptr<Key>  key_;
};

}
