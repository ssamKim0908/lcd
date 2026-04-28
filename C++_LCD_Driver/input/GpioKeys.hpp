#pragma once
#include "../interface/IKeys.hpp"
#include <memory>

class GpioRead;

class GpioKeys : public IKeys
{
private:
    std::unique_ptr<GpioRead> reader;
public:
    GpioKeys(std::unique_ptr<GpioRead> reader);
    ~GpioKeys() override;

    GpioKeys(const GpioKeys&)            = delete;
    GpioKeys& operator=(const GpioKeys&) = delete;

    KeyEvent next_event() override;
};
