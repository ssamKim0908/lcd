#include "UdsChannel.hpp"
#ifdef TARGET_DEVICE
#include <sys/socket.h>
#include <cerrno>
#include <system_error>
#endif

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

void UdsChannel::recv(util::Span<std::byte> data)
{
#ifdef TARGET_DEVICE
    size_t received = 0;
    while (received < data.size())
    {
        ssize_t n = ::recv(fd_, data.data() + received, data.size() - received, 0);
        if (n < 0)
        {
            throw std::system_error(errno, std::generic_category(), "UDS recv");
        }
        if (n == 0)
        {
            throw std::runtime_error("UDS connection closed by peer");
        }
        received += static_cast<size_t>(n);
    }
#else
    std::cout << "Receiving " << data.size() << " bytes on UDS fd: " << fd_ << std::endl;
#endif
}