#pragma once
#include "../interface/IChannel.hpp"

class UdsChannel : public IChannel
{
private:
    int fd_ = -1;
public:
    explicit UdsChannel(int fd);
    ~UdsChannel() override;

    UdsChannel(const UdsChannel&)            = delete;
    UdsChannel& operator=(const UdsChannel&) = delete;

    void send   (util::Span<const std::byte> data) override;
    void recv   (util::Span<std::byte>       data) override;
    int  fd     () const override { return fd_; }
};
