#include "IpcClient.hpp"
#include "../../interface/IChannel.hpp"
#include "../../shared/Packet.hpp"
#include <cstring>
#include <stdexcept>

namespace sdk::internal
{

IpcClient::IpcClient(std::unique_ptr<IChannel> channel)
    : channel_(std::move(channel))
{}

IpcClient::~IpcClient() = default;

void IpcClient::send_bytes(util::Span<const std::byte> data)
{
    channel_->send(data);
}

KeyEvent IpcClient::recv_key()
{
    RecvResult r = channel_->recv();
    if (r.status == RecvResult::Status::Closed)
    {
        throw std::runtime_error("IpcClient::recv_key: server closed connection");
    }
    if (r.packet.data.size() < sizeof(KeyEvent))
    {
        throw std::runtime_error("IpcClient::recv_key: short packet");
    }
    KeyEvent ev;
    std::memcpy(&ev, r.packet.data.data(), sizeof(ev));
    return ev;
}

}
