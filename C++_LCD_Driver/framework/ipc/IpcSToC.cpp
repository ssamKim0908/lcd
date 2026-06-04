#include "IpcSToC.hpp"
#include "../../interface/IChannel.hpp"
#include "../../shared/MsgType.hpp"

#include <utility>

IpcSToC::IpcSToC()
{}

IpcSToC::~IpcSToC() = default;

SendStatus IpcSToC::send_key(KeyEvent ev, ISender& sender_)
{
    writer_.clear();
    writer_.put_u8(static_cast<uint8_t>(shared::S2C::KeyEvent));
    writer_.put_u8(static_cast<uint8_t>(ev.key));
    writer_.put_u8(static_cast<uint8_t>(ev.state));
    return sender_.send(writer_.bytes());
}
