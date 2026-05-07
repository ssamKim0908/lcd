#include "UdsChannel.hpp"
#include "../shared/Packet.hpp"
#include <iostream>
#include <unistd.h>
#ifdef TARGET_DEVICE
#include <sys/socket.h>
#include <cerrno>
#include <system_error>
#endif

namespace { constexpr std::size_t MAX_PACKET = 256; }

UdsChannel::UdsChannel(int fd_) : fd_(fd_)
{
    std::cout << "Opened UDS channel fd: " << fd_ << std::endl;
}

UdsChannel::~UdsChannel()
{
    if (fd_ >= 0)
    {
        std::cout << "Closing UDS channel fd: " << fd_ << std::endl;
        ::close(fd_);
    }
}

void UdsChannel::send(util::Span<const std::byte> data)
{
#ifdef TARGET_DEVICE
    size_t sent = 0;
    while (sent < data.size())
    {
        ssize_t n = ::send(fd_, data.data() + sent, data.size() - sent, 0);
        if (n < 0)
        {
            throw std::system_error(errno, std::generic_category(), "UDS send");
        }
        sent += static_cast<size_t>(n);
    }
#else
    std::cout << "Sending " << data.size() << " bytes on UDS fd: " << fd_ << std::endl;
#endif
}

Packet UdsChannel::recv()
{
#ifdef TARGET_DEVICE
    Packet pkt;
    pkt.data.resize(MAX_PACKET);

    ssize_t n = ::recv(fd_, pkt.data.data(), pkt.data.size(), 0);
    if (n < 0)
    {
        throw std::system_error(errno, std::generic_category(), "UDS recv");
    }
    if (n == 0)
    {
        throw std::runtime_error("UDS connection closed by peer");
    }

    pkt.data.resize(static_cast<std::size_t>(n));
    return pkt;
#else
    std::cout << "Receiving one message on UDS fd: " << fd_ << std::endl;
    return Packet{};
#endif
}