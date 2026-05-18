#pragma once
#include "../../interface/IProxyServer.hpp"
#include "../../util/IpcSerializer.hpp"
#include <cstddef>
#include <memory>
#include <vector>

class ISender;

class IpcSToC : public IProxyServerToClient
{
private:
    std::vector<std::byte>    buffer_;
    util::ipc::Writer         writer_;

public:
    explicit IpcSToC();
    ~IpcSToC() override;

    IpcSToC(const IpcSToC&)            = delete;
    IpcSToC& operator=(const IpcSToC&) = delete;

    SendStatus send_key(KeyEvent ev, const std::shared_ptr<ISender>& sender_) override;
};
