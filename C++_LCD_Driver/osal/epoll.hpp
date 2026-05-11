#pragma once
#include "../interface/IPoller.hpp"

class Epoll : public IPoller
{
private:
    int epoll_fd = -1;
public:
    Epoll();
    ~Epoll() override;

    Epoll(const Epoll&)            = delete;
    Epoll& operator=(const Epoll&) = delete;
    Epoll(Epoll&&)                 = delete;
    Epoll& operator=(Epoll&&)      = delete;

    void        add (int fd, uint32_t events)   override;
    void        del (int fd)                    override;
    PollResult        wait()                          override;
};
