#pragma once
#include "../../interface/IServer.hpp"
#include <string>

class UdsServer : public IServer
{
private:
    int         fd_ = -1;
    std::string path;
public:
    UdsServer(const std::string& path);
    ~UdsServer() override;

    UdsServer(const UdsServer&)            = delete;
    UdsServer& operator=(const UdsServer&) = delete;

    std::unique_ptr<IChannel> accept() override;
    int                       fd() const override { return fd_; }
};
