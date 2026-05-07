#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

struct Packet
{
    uint32_t               len;
    std::vector<std::byte> data;
};
