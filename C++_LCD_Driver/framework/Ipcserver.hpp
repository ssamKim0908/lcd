#pragma once
#include <memory>
#include <thread>

class IServer;
class IChannel;

class Server
{
private:
    std::unique_ptr<IServer>    server;
public:
    Server(std::unique_ptr<IServer> server);
    ~Server();

    Server(const Server&)            = delete;
    Server& operator=(const Server&) = delete;

    void accept();
};
