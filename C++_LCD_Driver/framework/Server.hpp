#pragma once
#include <memory>
#include <stack>

class IServer;
class IChannel;
class IPoller;
class IKeys;
class IProxyServerFromClient;
class FocusStack;
class Rasterizer;

class Server
{
private:
    std::unique_ptr<IServer>                        server_;
    std::unique_ptr<IKeys>                          keys_;
    std::unique_ptr<IPoller>                        poller_;
    std::unique_ptr<IProxyServerFromClient>         proxy_;
    FocusStack                                      focus_;
    bool                                            running_ = true;

private:
    void on_accept     ();
    void on_key        ();
    void on_recv       (int fd);
    void disconnect_top();

public:
    Server(std::unique_ptr<IServer>    server,
           std::unique_ptr<IPoller>    poller,
           std::unique_ptr<IKeys>      keys,
           std::shared_ptr<Rasterizer> rasterizer);
    ~Server();

    Server(const Server&)            = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&)                 = delete;
    Server& operator=(Server&&)      = delete;

    void run();
};

class FocusStack
{
private:
    std::stack<std::shared_ptr<IChannel>> stack_;
public:
    FocusStack();
    ~FocusStack();

    FocusStack(const FocusStack&)            = delete;
    FocusStack& operator=(const FocusStack&) = delete;

    void                       push (std::unique_ptr<IChannel> channel);
    void                       pop  ();
    bool                       empty() const;
    std::shared_ptr<IChannel>  top  () const;
};
