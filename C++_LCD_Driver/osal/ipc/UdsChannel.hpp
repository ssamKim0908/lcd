#pragma once
#include "../../interface/IChannel.hpp"

struct RecvResult;

class UdsChannel : public IChannel
{
private:
    int fd_ = -1;
public:
    explicit UdsChannel(int fd);
    ~UdsChannel() override;

    UdsChannel(const UdsChannel&)            = delete;
    UdsChannel& operator=(const UdsChannel&) = delete;

    SendStatus send (util::Span<const std::byte> data) override;
    RecvResult recv ()                                 override;
    int        fd   () const override { return fd_; }
};
