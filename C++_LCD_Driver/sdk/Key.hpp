#pragma once
#include "../shared/KeyEvent.hpp"
#include <memory>

namespace sdk
{

class KeyImpl;

class Key
{
public:
    explicit Key(std::unique_ptr<KeyImpl> impl);
    ~Key();

    Key(const Key&)            = delete;
    Key& operator=(const Key&) = delete;

    KeyEvent get_key();

private:
    std::unique_ptr<KeyImpl> impl_;
};

}
