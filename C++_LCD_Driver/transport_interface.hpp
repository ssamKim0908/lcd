#pragma once
#include "pch.h"

class ITransport_read
{
public:
    virtual void read() = 0;
};

class ITransport_write
{
public:
    virtual void write() = 0;
};

class ITransport_ioctl
{
public:
    virtual void ioctl() = 0;
};

class Fd_transport
{
protected:
    int fd = -1;

    Fd_transport(const std::string &device, int flags)
    {
        fd = open(device.c_str(), flags);
        if (fd < 0)
        {
            throw std::runtime_error("Failed to open device");
        }
    };

public:
    virtual ~Fd_transport()
    {
        if (fd >= 0)
        {
            std::cout << "Closing file descriptor: " << fd << std::endl;
            close(fd);
        }
    }
};

