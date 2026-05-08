#include "framework/Ipcserver.hpp"
#include "framework/Rasterizer.hpp"
#include "ipc/UdsServer.hpp"
#include "display/St7789Lcd.hpp"
#include "display/LcdWriter.hpp"
#include "shared/Paths.hpp"
#include "hal/epoll.hpp"
#include "hal/gpio.hpp"
#include "hal/spi.hpp"
#include "input/GpioKeys.hpp"

#include <cerrno>
#include <csignal>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <system_error>

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef APP_MANAGER_PATH
#define APP_MANAGER_PATH "/usr/local/bin/app_manager"
#endif

namespace
{

void launch_app_manager()
{
    pid_t pid = ::fork();
    if (pid < 0)
        throw std::system_error(errno, std::generic_category(), "fork app_manager");

    if (pid == 0)
    {
        const char* path = APP_MANAGER_PATH;
        ::execl(path, path, static_cast<char*>(nullptr));
        std::cerr << "[server] exec " << path << " failed: "
                  << std::strerror(errno) << std::endl;
        ::_exit(127);
    }

    std::cout << "[server] launched app_manager pid=" << pid << std::endl;
}

}

int main()
{
    std::signal(SIGPIPE, SIG_IGN);
    std::signal(SIGCHLD, SIG_IGN);   // app_manager 자식 zombie 방지

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
    auto keys       = std::make_unique<GpioKeys>(std::move(reader));

    Server server(std::move(uds_server), std::move(poller),
                  std::move(keys), rasterizer);

    // --- Bring up app_manager (init's first child) ---
    launch_app_manager();

    // --- Reactor loop (blocks forever) ---
    server.run();

    return 0;
}
