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
    Packet pkt = channel_->recv();
    if (pkt.data.size() < sizeof(KeyEvent))
    {
        throw std::runtime_error("IpcClient::recv_key: short packet");
    }
    KeyEvent ev;
    std::memcpy(&ev, pkt.data.data(), sizeof(ev));
    return ev;
}

}
