#pragma once
#include "../interface/IChannel.hpp"

struct Packet;

class UdsChannel : public IChannel
{
private:
    int fd_ = -1;
public:
    explicit UdsChannel(int fd);
    ~UdsChannel() override;

    UdsChannel(const UdsChannel&)            = delete;
    UdsChannel& operator=(const UdsChannel&) = delete;

    void   send (util::Span<const std::byte> data) override;
    Packet recv ()                                 override;
    int    fd   () const override { return fd_; }
};
