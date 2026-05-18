#pragma once
#include "../../interface/IProxyClient.hpp"
#include <memory>

class IReceiver;

namespace sdk::internal
{

class IpcCFromS : public IProxyClientFromServer
{
private:
    std::shared_ptr<IReceiver> receiver_;

public:
    explicit IpcCFromS(std::shared_ptr<IReceiver> receiver);
    ~IpcCFromS() override;

    IpcCFromS(const IpcCFromS&)            = delete;
    IpcCFromS& operator=(const IpcCFromS&) = delete;

    KeyEvent recv_key() override;
};

}
