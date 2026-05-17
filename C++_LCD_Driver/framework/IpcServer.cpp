#include "Ipcserver.hpp"
#include "../interface/IServer.hpp"
#include "../interface/IChannel.hpp"
#include "../interface/IPoller.hpp"
#include "../interface/IKeys.hpp"
#include "../shared/Packet.hpp"
#include "../shared/KeyEvent.hpp"
#include "../util/span.hpp"
#include "CommandFactory.hpp"
#include <iostream>

//-----------------Server Private--------------------------
void Server::on_accept()
{
    auto channel = server_->accept();
    int  new_fd  = channel->fd();

    if (auto old_top = focus_->top())
        poller_->del(old_top->fd());

    focus_->push(std::move(channel));
    poller_->add(new_fd, static_cast<uint32_t>(PollEvents::In));

    std::cout << "client connected: fd=" << new_fd << std::endl;
}

void Server::on_key()
{
    KeyEvent k = keys_->next_event();
    auto top = focus_->top();
    if (!top) return;

    if (top->send(util::as_bytes(util::Span<const KeyEvent>(&k, 1))) == SendStatus::Closed)
        disconnect_top();
}

void Server::on_recv(int fd)
{
    auto top = focus_->top();
    if (!top || top->fd() != fd) return;

    RecvResult r = top->recv();
    switch (r.status)
    {
    case RecvResult::Status::Data:
    {
        std::cout << "recv done! size=" << r.packet.data.size() << std::endl;
        auto command = proxy_->create(r.packet);
        command->execute();
        break;
    }
    case RecvResult::Status::Closed:
        disconnect_top();
        break;
    }
}

void Server::disconnect_top()
{
    auto closing = focus_->top();
    if (!closing) return;

    int fd = closing->fd();
    poller_->del(fd);
    focus_->pop();

    if (auto new_top = focus_->top())
        poller_->add(new_top->fd(), static_cast<uint32_t>(PollEvents::In));

    std::cout << "client disconnected: fd=" << fd << std::endl;
}

//-----------------Server Public--------------------------

Server::Server(std::unique_ptr<IServer>    server,
               std::unique_ptr<IPoller>    poller,
               std::unique_ptr<IKeys>      keys,
               std::shared_ptr<Rasterizer> rasterizer)
    : server_ (std::move(server))
    , keys_   (std::move(keys))
    , poller_ (std::move(poller))
    , focus_  (std::make_unique<FocusStack>())
    , factory_(rasterizer)
{}

Server::~Server() = default;

void Server::run()
{
    poller_->add(server_->fd(), static_cast<uint32_t>(PollEvents::In));
    poller_->add(keys_->fd(),   static_cast<uint32_t>(PollEvents::In));

    while (running_)
    {
        PollResult ev = poller_->wait();
        if (ev.fd == IPoller::ERROR.fd) continue;
        if ((ev.events & static_cast<uint32_t>(PollEvents::In)) == 0) continue;

        if      (ev.fd == server_->fd()) on_accept();
        else if (ev.fd == keys_->fd())   on_key();
        else                             on_recv(ev.fd);
    }
}


//--------------------FocusStack--------------------------
FocusStack::FocusStack() = default;

FocusStack::~FocusStack()
{
    while (!stack_.empty()) stack_.pop();
}

void FocusStack::push(std::unique_ptr<IChannel> channel)
{
    stack_.push(std::shared_ptr<IChannel>(std::move(channel)));
}

void FocusStack::pop()
{
    if (!stack_.empty()) stack_.pop();
}

bool FocusStack::empty() const
{
    return stack_.empty();
}

std::shared_ptr<IChannel> FocusStack::top() const
{
    if (stack_.empty()) return nullptr;
    return stack_.top();
}
