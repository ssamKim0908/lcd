#pragma once
#include <cstddef>
#include <vector>

struct Packet
{
    std::vector<std::byte> data;
};

struct RecvResult
{
    enum class Status { Data, Closed };

    Status status;
    Packet packet;
};
