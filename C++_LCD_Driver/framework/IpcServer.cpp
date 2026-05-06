#include "Ipcserver.hpp"
#include "../interface/IServer.hpp"
#include "../interface/IChannel.hpp"
#include "../interface/IPoller.hpp"

//private
void Server::recv_loop()
{
    while(1)
    {
        if(focus_stack.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        PollResult result = poller->wait();

        if (result.fd != IPoller::ERROR.fd && (result.events & static_cast<uint32_t>(PollEvents::In)) != 0)
        {
            if (focus_stack.top()->fd() == result.fd)
            {
                std::byte buffer[1024];
                focus_stack.top()->recv(Span<std::byte>(buffer, sizeof(buffer)));
            }
        }
    }
}

void Server::send_loop()
{
    while(1)
    {
        if(focus_stack.empty()) continue;
    }    
}

//public
Server::Server(std::unique_ptr<IServer> server)
    : server(std::move(server))
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
        focus_stack.push(std::move(new_channel));

        uint32_t events = static_cast<uint32_t>(PollEvents::In);
        poller->add(focus_stack.top().get()->fd(), events);
    }
}