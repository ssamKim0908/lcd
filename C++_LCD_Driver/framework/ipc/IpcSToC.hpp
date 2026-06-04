#pragma once
#include "../../interface/IProxyServer.hpp"
#include "../../util/IpcSerializer.hpp"
#include <memory>

class ISender;

class IpcSToC : public IProxyServerToClient
{
private:
    util::ipc::Writer         writer_;

public:
    explicit IpcSToC();
    ~IpcSToC() override;

    IpcSToC(const IpcSToC&)            = delete;
    IpcSToC& operator=(const IpcSToC&) = delete;

    SendStatus send_key(KeyEvent ev, ISender& sender_) override;
};
