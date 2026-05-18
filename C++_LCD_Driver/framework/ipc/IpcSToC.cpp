#include "IpcSToC.hpp"
#include "../interface/IChannel.hpp"
#include "../shared/MsgType.hpp"
#include "../util/span.hpp"

#include <utility>

IpcSToC::IpcSToC(std::shared_ptr<ISender> sender)
    : sender_(std::move(sender)) {}

IpcSToC::~IpcSToC() = default;

SendStatus IpcSToC::on_key(KeyEvent ev)
{
    buffer_.clear();
    writer_.put_u8(buffer_, static_cast<uint8_t>(shared::S2C::KeyEvent));
    writer_.put_u8(buffer_, static_cast<uint8_t>(ev.key));
    writer_.put_u8(buffer_, static_cast<uint8_t>(ev.state));
    return sender_->send(util::Span<const std::byte>{ buffer_.data(), buffer_.size() });
}
