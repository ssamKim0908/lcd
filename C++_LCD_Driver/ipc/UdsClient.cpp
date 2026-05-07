#include "UdsClient.hpp"
#include "UdsChannel.hpp"
#include <iostream>
#include <unistd.h>
#ifdef TARGET_DEVICE
#include <sys/socket.h>
#include <sys/un.h>
#include <cerrno>
#include <cstring>
#include <system_error>
#endif

UdsClient::UdsClient(const std::string& path) : path(path) {}

std::unique_ptr<IChannel> UdsClient::connect()
{
#ifdef TARGET_DEVICE
    int fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "Failed to create UDS client socket");
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

    if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        ::close(fd);
        throw std::system_error(errno, std::generic_category(),
            "Failed to connect UDS socket: " + path);
    }

    std::cout << "Connected UDS client fd: " << fd << std::endl;
    return std::make_unique<UdsChannel>(fd);
#else
    std::cout << "Connecting UDS client to path: " << path << std::endl;
    return std::make_unique<UdsChannel>(-1);
#endif
}
