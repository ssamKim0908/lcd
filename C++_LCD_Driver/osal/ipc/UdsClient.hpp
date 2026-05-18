#pragma once
#include "../interface/IClient.hpp"
#include <string>

class UdsClient : public IClient
{
private:
    std::string path;
public:
    UdsClient(const std::string& path);
    ~UdsClient() override = default;

    UdsClient(const UdsClient&)            = delete;
    UdsClient& operator=(const UdsClient&) = delete;

    std::unique_ptr<IChannel> connect() override;
};
