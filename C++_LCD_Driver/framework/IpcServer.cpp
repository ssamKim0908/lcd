#include "Ipcserver.hpp"
#include "../interface/IServer.hpp"
#include "../interface/IChannel.hpp"
#include "../interface/IPoller.hpp"
#include "../interface/IKeys.hpp"
#include "../util/span.hpp"
#include <iostream>


//Server private
void Server::recv_loop()
{
    while(1)
    {
        if(focus_stack->empty()) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        PollResult result = poller->wait();

        if (result.fd != IPoller::ERROR.fd && (result.events & static_cast<uint32_t>(PollEvents::In)) != 0)
        {
            if (focus_stack->top()->fd() == result.fd)
            {
                std::byte buffer[1024];
                //focus_stack->top()->recv(util::Span<std::byte>(buffer, sizeof(buffer)));
                std::cout << "recv done!" << std::endl;
            }
        }
    }
}

void Server::send_loop()
{
    while(1)
    {
        auto key_input = keys->next_event();
        if(focus_stack->empty()) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        focus_stack->top()->send(util::as_bytes(key_input));
    }    
}

//Server public
Server::Server(std::unique_ptr<IServer> server,
               std::unique_ptr<IPoller> poller,
               std::unique_ptr<IKeys>   keys)
    : server(std::move(server))
    , poller(std::move(poller))
    , keys(std::move(keys))
    , focus_stack(std::make_unique<FocusStack>())
{
    recv_thread = std::thread(&Server::recv_loop, this);
    send_thread = std::thread(&Server::send_loop, this);
}

Server::~Server()
{
    if(recv_thread.joinable()) recv_thread.join();
    if(send_thread.joinable()) send_thread.join();
}

void Server::accept()
{
    while(1)
    {
        auto new_channel = server->accept();
        focus_stack->push(std::move(new_channel));

        uint32_t events = static_cast<uint32_t>(PollEvents::In);
        poller->add(focus_stack->top()->fd(), events);
    }
}

//Focus Stack Private

//Focus Stack Pulbic
FocusStack::FocusStack() = default;

FocusStack::~FocusStack()
{
    while(!focus_stack.empty())
    {
        focus_stack.pop();
    }
}

void FocusStack::push(std::unique_ptr<IChannel> channel)
{
    std::lock_guard<std::mutex> lock(mtx);
    focus_stack.push(std::shared_ptr<IChannel>(std::move(channel)));
}

void FocusStack::pop()
{
    std::lock_guard<std::mutex> lock(mtx);
    focus_stack.pop();
}

bool FocusStack::empty()
{
    return focus_stack.empty();
}

std::shared_ptr<IChannel> FocusStack::top()
{
    std::lock_guard<std::mutex> lock(mtx);
    return focus_stack.top();
}