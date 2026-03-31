#pragma once
#include "pch.h"
#include "communication_interface.hpp"

class Fd_Spi : public IWrite, public IIoctl
{
private:
    Fd_Object fd_t;
public:
    Fd_Spi(const std::string& device, int flags) : fd_t{device, flags} {};

    void write(const span<uint8_t> buffer) override;
    void ioctl(uint32_t flag, void* arg) override;
};