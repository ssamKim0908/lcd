#include "epoll.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <cerrno>
#include <system_error>

namespace {

uint32_t to_epoll(uint32_t events)
{
    uint32_t e = 0;
    if (events & static_cast<uint32_t>(PollEvents::In))  e |= EPOLLIN;
    if (events & static_cast<uint32_t>(PollEvents::Out)) e |= EPOLLOUT;
    return e;
}

uint32_t from_epoll(uint32_t e)
{
    uint32_t out = 0;
    if (e & EPOLLIN)  out |= static_cast<uint32_t>(PollEvents::In);
    if (e & EPOLLOUT) out |= static_cast<uint32_t>(PollEvents::Out);
    return out;
}

}  // namespace

Epoll::Epoll()
{
    epoll_fd = ::epoll_create1(EPOLL_CLOEXEC);
    if (epoll_fd < 0)
    {
        throw std::system_error(errno, std::generic_category(), "epoll_create1");
    }
}

Epoll::~Epoll()
{
    if (epoll_fd >= 0) ::close(epoll_fd);
}

void Epoll::add(int fd, uint32_t events)
{
    epoll_event ev{};
    ev.events  = to_epoll(events);
    ev.data.fd = fd;
    if (::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) < 0)
    {
        throw std::system_error(errno, std::generic_category(), "epoll_ctl ADD");
    }
}

void Epoll::del(int fd)
{
    if (::epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) < 0)
    {
        throw std::system_error(errno, std::generic_category(), "epoll_ctl DEL");
    }
}

void Epoll::wait()
{
    epoll_event ev{};
    int n = ::epoll_wait(epoll_fd, &ev, 1, -1);
    if (n < 0)
    {
        throw std::system_error(errno, std::generic_category(), "epoll_wait");
    }
}
