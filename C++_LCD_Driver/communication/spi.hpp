#pragma once
#include "pch.h"
#include "ICommunication.hpp"

class FdSpi : public IWrite
{
private:
    FdObject FdT;
public:
    FdSpi(const std::string& device, int flags) : FdT{device, flags} {};

    void write(const span<uint8_t> buffer) override;
};