#pragma once

#include "../include/common.h"
#include "../include/span.hpp"

class IRead
{
public:
    virtual void read(span<std::byte> buffer) = 0;
    virtual ~IRead() = default;
};

class IWrite
{
public:
    virtual void write(span<const std::byte> buffer) = 0;
    virtual ~IWrite() = default;
};

class FdObject
{
private:
    int Fd = -1;
public:
    FdObject(const std::string &device, int flags)
    {
        Fd = open(device.c_str(), flags);
        if (Fd < 0)
        {
            throw std::runtime_error("Failed to open device");
        }
        else
        {
            std::cout << "Opened file descriptor: " << Fd << std::endl;
        }
    };


    FdObject(const FdObject&)             = delete;
    FdObject& operator=(const FdObject&)  = delete;

    FdObject(FdObject&&other)             = delete;
    FdObject& operator=(FdObject&&other)  = delete;

    int get_fd() const { return Fd; }

    ~FdObject()
    {
        if (Fd >= 0)
        {
            std::cout << "Closing file descriptor: " << Fd << std::endl;
            close(Fd);
        }
    }
};
