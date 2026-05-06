#include "UdsServer.hpp"
#include "UdsChannel.hpp"
#include "../include/common.h"
#ifdef TARGET_DEVICE
#include <sys/socket.h>
#include <sys/un.h>
#include <cerrno>
#include <cstring>
#include <system_error>
#endif

UdsServer::UdsServer(const std::string& path) : path(path)
{
#ifdef TARGET_DEVICE
    fd_ = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd_ < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "Failed to create UDS server socket");
    }
    std::cout << "Opened UDS server fd: " << fd_ << std::endl;

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

    ::unlink(path.c_str());

    if (::bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "Failed to bind UDS socket: " + path);
    }

    if (::listen(fd_, 5) < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "Failed to listen UDS socket: " + path);
    }
#endif
}

UdsServer::~UdsServer()
{
    if (fd_ >= 0)
    {
        std::cout << "Closing UDS server fd: " << fd_ << std::endl;
        ::close(fd_);
#ifdef TARGET_DEVICE
        ::unlink(path.c_str());
#endif
    }
}

std::unique_ptr<IChannel> UdsServer::accept()
{
#ifdef TARGET_DEVICE
    int client_fd = ::accept(fd_, nullptr, nullptr);
    if (client_fd < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "Failed to accept UDS connection");
    }
    return std::make_unique<UdsChannel>(client_fd);
#else
    std::cout << "Accepting UDS connection on path: " << path << std::endl;
    return std::make_unique<UdsChannel>(-1);
#endif
}
