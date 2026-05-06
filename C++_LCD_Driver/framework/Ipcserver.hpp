#pragma once
#include <memory>
#include <thread>
#include <stack>

class IServer;
class IChannel;
class IPoller;

class Server
{
private:
    std::unique_ptr<IServer>    server;
    std::stack<std::unique_ptr<IChannel>> focus_stack;
    std::unique_ptr<IPoller>   poller;
    std::thread recv_thread;
    std::thread send_thread;
private:
    void recv_loop();
    void send_loop();
public:
    Server(std::unique_ptr<IServer> server);
    ~Server();

    Server(const Server&)            = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&)                 = delete;
    Server& operator=(Server&&)      = delete;

    void accept();
};
