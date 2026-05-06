#pragma once
#include <cstdint>

//일단 필요한 것만 만들어 두자.
enum class PollEvents : uint32_t
{
    In  = 1u << 0,
    Out = 1u << 1,
};

class IPoller
{
public:
    virtual ~IPoller() = default;

    virtual void        add (int fd, uint32_t events)   = 0;
    virtual void        del (int fd)                    = 0;
    virtual void        wait()                          = 0;
};
