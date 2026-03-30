#pragma once
#include "pch.h"

class ITransport_Read
{
public:
    virtual void Read(uint8_t *buffer, uint32_t length) = 0;
};

class ITransport_Write
{
public:
    virtual void Write(const uint8_t *buffer, uint32_t length) = 0;
};

class ITransport_Ioctl
{
public:
    virtual void Ioctl(uint32_t flag, void *arg) = 0;
};

class Fd_Transport
{
private:
    int fd = -1;

public:
    Fd_Transport(const std::string &device, int flags)
    {
        fd = open(device.c_str(), flags);
        if (fd < 0)
        {
            throw std::runtime_error("Failed to open device");
        }
    };

    virtual ~Fd_Transport()
    {
        if (fd >= 0)
        {
            std::cout << "Closing file descriptor: " << fd << std::endl;
            close(fd);
        }
    }

    Fd_Transport(const Fd_Transport &) = delete;
    Fd_Transport& operator=(const Fd_Transport &) = delete;

    Fd_Transport(Fd_Transport &&other) noexcept : fd(other.fd) 
    {
        other.fd = -1;
    }

    Fd_Transport& operator=(Fd_Transport &&other) noexcept
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
};
