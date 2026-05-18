#pragma once
#include "../interface/IProxyServer.hpp"
#include "../util/IpcSerializer.hpp"
#include <cstddef>
#include <memory>
#include <vector>

class ISender;

class IpcSToC : public IProxyServerToClient
{
private:
    std::shared_ptr<ISender>  sender_;
    std::vector<std::byte>    buffer_;
    util::ipc::Writer         writer_;

public:
    explicit IpcSToC(std::shared_ptr<ISender> sender);
    ~IpcSToC() override;

    IpcSToC(const IpcSToC&)            = delete;
    IpcSToC& operator=(const IpcSToC&) = delete;

    SendStatus on_key(KeyEvent ev) override;
};
