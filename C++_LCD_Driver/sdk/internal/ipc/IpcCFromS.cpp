#include "IpcCFromS.hpp"
#include "../../../interface/IChannel.hpp"
#include "../../../shared/Packet.hpp"
#include "../../../shared/MsgType.hpp"
#include "../../../util/IpcSerializer.hpp"

#include <stdexcept>
#include <utility>

namespace sdk::internal
{

IpcCFromS::IpcCFromS(std::shared_ptr<IReceiver> receiver)
    : receiver_(std::move(receiver)) {}

IpcCFromS::~IpcCFromS() = default;

KeyEvent IpcCFromS::recv_key()
{
    RecvResult result = receiver_->recv();
    if (result.status == RecvResult::Status::Closed)
        throw std::runtime_error("IpcCFromS: channel closed");

    util::ipc::Reader r(result.packet.data.data(), result.packet.data.size());
    const auto type = static_cast<shared::S2C>(r.get_u8());
    if (type != shared::S2C::KeyEvent)
        throw std::runtime_error("IpcCFromS: unexpected S2C type");

    KeyEvent ev;
    ev.key   = static_cast<Key>     (r.get_u8());
    ev.state = static_cast<KeyState>(r.get_u8());
    return ev;
}

}
