#pragma once
#include "pch.h"
#include "include/span.hpp"

class IRead
{
public:
    virtual void read(span<uint8_t> buffer) = 0;
    virtual ~IRead() = default;
};

class IWrite
{
public:
    virtual void write(const span<uint8_t> buffer) = 0;
    virtual ~IWrite() = default;
};

class IIoctl
{
public:
    virtual void ioctl(uint32_t flag, void *arg) = 0;
    virtual ~IIoctl() = default;
};

//일단 만들어 둠. 나중에 spi에서 쓸 수도 있으니, 필요하다면 이런 방식으로 쓰겠다는 뜻.
class IReadWrite
{
public:
    virtual void read_write(span<uint8_t> rx, span<uint8_t> tx) = 0;
    virtual ~IReadWrite() = default;
};

class Fd_Object
{
private:
    int fd = -1;
public:
    Fd_Object(const std::string &device, int flags)
    {
        fd = open(device.c_str(), flags);
        if (fd < 0)
        {
            throw std::runtime_error("Failed to open device");
        }
    };

    
    Fd_Object(const Fd_Object&) = delete;
    Fd_Object& operator=(const Fd_Object&) = delete;
    
    Fd_Object(Fd_Object&&other) noexcept : fd(other.fd) 
    {
        other.fd = -1;
    }
    
    Fd_Object& operator=(Fd_Object&&other) noexcept
    {
        if (this != &other)
        {
            if (fd >= 0)
            {
                std::cout << "Closing file descriptor: " << fd << std::endl;
                close(fd);
            }
            fd = other.fd;
            other.fd = -1;
        }
        return *this;
    }
     
    int get_fd() const { return fd; }
    
    ~Fd_Object()
    {
        if (fd >= 0)
        {
            std::cout << "Closing file descriptor: " << fd << std::endl;
            close(fd);
        }
    }
};
