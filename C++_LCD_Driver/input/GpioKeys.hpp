#pragma once
#include "../interface/IKeys.hpp"
#include <memory>
#include <map>

class GpioRead;
enum class InputKey : uint32_t;

class GpioKeys : public IKeys
{
private:
    std::unique_ptr<GpioRead>   reader;
    std::map<InputKey, Key>     key_map;
public:
    GpioKeys(std::unique_ptr<GpioRead> reader);
    ~GpioKeys() override;

    GpioKeys(const GpioKeys&)            = delete;
    GpioKeys& operator=(const GpioKeys&) = delete;

    KeyEvent next_event() override;
};
