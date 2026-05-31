#include "framework/Server.hpp"
#include "framework/Rasterizer.hpp"
#include "osal/ipc/UdsServer.hpp"
#include "osal/facade/display/St7789Lcd.hpp"
#include "osal/facade/display/LcdWriter.hpp"
#include "osal/facade/input/GpioKeys.hpp"
#include "osal/facade/input/FakeKeys.hpp"
#include "osal/process/NonBlockingProcessLauncher.hpp"
#include "osal/epoll.hpp"
#include "osal/gpio.hpp"
#include "osal/spi.hpp"
#include "interface/IKeys.hpp"
#include "shared/SocketPaths.hpp"

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include <fcntl.h>

#ifndef APP_MANAGER_PATH
#define APP_MANAGER_PATH "/usr/local/bin/app_manager"
#endif

int main()
{
    std::signal(SIGPIPE, SIG_IGN);
    std::signal(SIGCHLD, SIG_IGN);   // NonBlockingProcessLauncher 자식 zombie 자동 reap

    const std::string sock_path = shared::SOCK_PATH;

    // --- HAL ---
    auto chip       = std::make_unique<Gpio>("/dev/gpiochip0", O_RDWR);
    auto reader     = std::make_unique<GpioRead>(chip->get_fd());
    auto write_gpio = std::make_unique<GpioWrite>(chip->get_fd());
    auto spi        = std::make_unique<Spi>("/dev/spidev0.0", O_RDWR);

    // --- Display chain ---
    auto lcd_writer = std::make_unique<LcdWriter>(std::move(spi), std::move(write_gpio));
    auto lcd        = std::make_unique<St7789Lcd>(std::move(lcd_writer));
    lcd->init();
    auto rasterizer = std::make_shared<Rasterizer>(std::move(lcd));

    // --- Server ---
    auto uds_server = std::make_unique<UdsServer>(sock_path);
    auto poller     = std::make_unique<Epoll>();

    std::unique_ptr<IKeys> keys;
    if (const char* ms = std::getenv("FAKE_KEYS_MS"))
    {
        keys = std::make_unique<FakeKeys>(std::atoi(ms));
        std::cout << "[server] FakeKeys " << ms << "ms (GPIO 입력 무시)" << std::endl;
    }
    else
    {
        keys = std::make_unique<GpioKeys>(std::move(reader));
    }

    Server server(std::move(uds_server), std::move(poller),
                  std::move(keys), rasterizer);

    // --- Bring up app_manager (init's first child) ---
    auto launcher = std::make_unique<NonBlockingProcessLauncher>();
    launcher->launch(APP_MANAGER_PATH);
    std::cout << "[server] launched app_manager" << std::endl;

    // --- Reactor loop (blocks forever) ---
    server.run();

    return 0;
}
