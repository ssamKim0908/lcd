#pragma once
#include <chrono>
#include <thread>
#include <cstdint>

namespace utils::time
{
    inline void sleep_ms(uint32_t ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    inline void sleep_us(uint32_t us)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(us));
    }
}
