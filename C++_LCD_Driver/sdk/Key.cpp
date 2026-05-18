#include "Key.hpp"
#include "internal/KeyImpl.hpp"

#include <utility>

namespace sdk
{

Key::Key(std::unique_ptr<KeyImpl> impl)
    : impl_(std::move(impl)) {}

Key::~Key() = default;

KeyEvent Key::get_key()
{
    return impl_->get_key();
}

}
