#include "framework/Ipcserver.hpp"
#include "ipc/UdsServer.hpp"
#include "ipc/UdsClient.hpp"
#include "interface/IChannel.hpp"
#include "interface/IKeys.hpp"
#include "shared/Packet.hpp"
#include "hal/epoll.hpp"
#include "hal/gpio.hpp"
#include "input/GpioKeys.hpp"
#include "util/span.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <cstring>
#include <fcntl.h>

int main()
{
    const std::string sock_path = "/tmp/lcd-test.sock";

    // --- Server side ---
    auto uds    = std::make_unique<UdsServer>(sock_path);
    auto poller = std::make_unique<Epoll>();
    auto chip   = std::make_unique<Gpio>("/dev/gpiochip0", O_RDWR);
    auto reader = std::make_unique<GpioRead>(chip->get_fd());
    auto keys   = std::make_unique<GpioKeys>(std::move(reader));

    Server server(std::move(uds), std::move(poller), std::move(keys));

    // accept loop is its own thread
    std::thread accept_thread([&]{ server.accept(); });

    // give the listener a moment
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // --- Client side (same process, connects to the server) ---
    UdsClient client(sock_path);
    auto channel = client.connect();

    std::cout << "[client] connected. press buttons..." << std::endl;
    while (true)
    {
        Packet pkt = channel->recv();
        KeyEvent ev;
        std::memcpy(&ev, pkt.data.data(), sizeof(ev));
        std::cout << "[client] key=" << static_cast<int>(ev.key)
                  << " state=" << static_cast<int>(ev.state) << std::endl;
    }

    accept_thread.join();
    return 0;
}