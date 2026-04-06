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

    
    Fd_Object(const Fd_Object&)             = delete;
    Fd_Object& operator=(const Fd_Object&)  = delete;
    
    Fd_Object(Fd_Object&&other)             = delete;
    Fd_Object& operator=(Fd_Object&&other)  = delete;

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
