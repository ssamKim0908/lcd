#pragma once
#include "../interface/IChannel.hpp"

class UdsChannel : public IChannel
{
private:
    int fd = -1;
public:
    explicit UdsChannel(int fd);
    ~UdsChannel() override;

    UdsChannel(const UdsChannel&)            = delete;
    UdsChannel& operator=(const UdsChannel&) = delete;

    void send(Span<const std::byte> data) override;
    void recv(Span<std::byte>       data) override;
};
