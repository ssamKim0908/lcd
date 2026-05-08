#include "Ipcserver.hpp"
#include "../interface/IServer.hpp"
#include "../interface/IChannel.hpp"
#include "../interface/IPoller.hpp"
#include "../interface/IKeys.hpp"
#include "../shared/Packet.hpp"
#include "../util/span.hpp"
#include "CommandFactory.hpp"
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
        if (result.fd == IPoller::ERROR.fd) continue;
        if ((result.events & static_cast<uint32_t>(PollEvents::In)) == 0) continue;

        auto top = focus_stack->top();
        if (!top || top->fd() != result.fd) continue;

        RecvResult r = top->recv();
        switch (r.status)
        {
        case RecvResult::Status::Data:
        {
            std::cout << "recv done! size=" << r.packet.data.size() << std::endl;
            auto command = simple_command_factory.create(r.packet);
            command->execute();
            break;
        }
        case RecvResult::Status::Closed:
        {
            std::cout << "client closed fd: " << top->fd() << std::endl;
            poller->del(top->fd());
            focus_stack->pop();
            break;
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
               std::unique_ptr<IKeys>   keys,
            std::shared_ptr<Rasterizer> receiver)
    : server(std::move(server))
    , poller(std::move(poller))
    , keys(std::move(keys))
    , focus_stack(std::make_unique<FocusStack>())
    , simple_command_factory(receiver)
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