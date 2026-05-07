#pragma once
#include <memory>
#include <thread>
#include <stack>
#include <mutex>

class IServer;
class IChannel;
class IPoller;
class IKeys;
class FocusStack;

class Server
{
private:
    std::unique_ptr<IServer> server;
    std::unique_ptr<FocusStack> focus_stack;
    std::unique_ptr<IPoller> poller;
    std::unique_ptr<IKeys> keys;
    
    std::thread recv_thread;
    std::thread send_thread;

private:
    void recv_loop();
    void send_loop();

public:
    Server(std::unique_ptr<IServer> server,
           std::unique_ptr<IPoller> poller,
           std::unique_ptr<IKeys>   keys);
    ~Server();

    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;
    Server(Server &&) = delete;
    Server &operator=(Server &&) = delete;

    void accept();
};

class FocusStack
{
private:
    std::stack<std::shared_ptr<IChannel>> focus_stack;
    std::mutex mtx;
public:
    FocusStack();
    ~FocusStack();

    FocusStack(const FocusStack&) = delete;
    FocusStack& operator=(const FocusStack&) = delete;

    void push(std::unique_ptr<IChannel> channel);
    void pop();
    bool empty();
    std::shared_ptr<IChannel> top();
};