#pragma once
#include "../../shared/KeyEvent.hpp"
#include <memory>

class IProxyClientFromServer;

namespace sdk
{

class KeyImpl
{
public:
    explicit KeyImpl(std::unique_ptr<IProxyClientFromServer> proxy);
    ~KeyImpl();

    KeyImpl(const KeyImpl&)            = delete;
    KeyImpl& operator=(const KeyImpl&) = delete;

    KeyEvent get_key();

private:
    std::unique_ptr<IProxyClientFromServer> proxy_;
};

}
