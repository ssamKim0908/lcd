#include "framework/Ipcserver.hpp"
#include "framework/Rasterizer.hpp"
#include "ipc/UdsServer.hpp"
#include "ipc/UdsClient.hpp"
#include "display/St7789Lcd.hpp"
#include "display/LcdWriter.hpp"
#include "interface/IChannel.hpp"
#include "interface/IKeys.hpp"
#include "shared/Packet.hpp"
#include "shared/DrawCommand.hpp"
#include "hal/epoll.hpp"
#include "hal/gpio.hpp"
#include "hal/spi.hpp"
#include "input/GpioKeys.hpp"
#include "util/span.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <cstring>
#include <cstdint>
#include <vector>
#include <fcntl.h>

namespace
{

constexpr uint16_t COLOR_BLACK  = 0x0000;
constexpr uint16_t COLOR_WHITE  = 0xFFFF;
constexpr uint16_t COLOR_RED    = 0xF800;
constexpr uint16_t COLOR_GREEN  = 0x07E0;
constexpr uint16_t COLOR_BLUE   = 0x001F;
constexpr uint16_t COLOR_YELLOW = 0xFFE0;
constexpr uint16_t COLOR_CYAN   = 0x07FF;

template <typename T>
void put(std::vector<std::byte>& v, T x)
{
    const auto* p = reinterpret_cast<const std::byte*>(&x);
    v.insert(v.end(), p, p + sizeof(T));
}

std::vector<std::byte> encode_clear(uint16_t color)
{
    std::vector<std::byte> v;
    put(v, static_cast<uint8_t>(shared::DrawCommand::Clear));
    put(v, color);
    return v;
}

std::vector<std::byte> encode_fill_rect(int32_t x, int32_t y,
                                        int32_t w, int32_t h,
                                        uint16_t color)
{
    std::vector<std::byte> v;
    put(v, static_cast<uint8_t>(shared::DrawCommand::FillRect));
    put(v, x); put(v, y);
    put(v, w); put(v, h);
    put(v, color);
    return v;
}

std::vector<std::byte> encode_fill_circle(int32_t cx, int32_t cy, int32_t r,
                                          uint16_t color)
{
    std::vector<std::byte> v;
    put(v, static_cast<uint8_t>(shared::DrawCommand::FillCircle));
    put(v, cx); put(v, cy); put(v, r);
    put(v, color);
    return v;
}

std::vector<std::byte> encode_render()
{
    std::vector<std::byte> v;
    put(v, static_cast<uint8_t>(shared::DrawCommand::Render));
    return v;
}

void send_packet(IChannel& ch, const std::vector<std::byte>& data)
{
    ch.send(util::Span<const std::byte>(data.data(), data.size()));
}

// 한 frame = 화면 그리기 명령들 + render
void draw_frame(IChannel& ch, int frame)
{
    switch (frame % 5)
    {
    case 0:
        send_packet(ch, encode_clear(COLOR_RED));
        break;
    case 1:
        send_packet(ch, encode_clear(COLOR_GREEN));
        break;
    case 2:
        send_packet(ch, encode_clear(COLOR_BLUE));
        send_packet(ch, encode_fill_rect(60, 60, 120, 120, COLOR_WHITE));
        break;
    case 3:
        send_packet(ch, encode_clear(COLOR_BLACK));
        send_packet(ch, encode_fill_circle(120, 120, 80, COLOR_YELLOW));
        break;
    case 4:
        send_packet(ch, encode_clear(COLOR_CYAN));
        send_packet(ch, encode_fill_rect(40, 40, 160, 160, COLOR_BLACK));
        send_packet(ch, encode_fill_circle(120, 120, 50, COLOR_RED));
        break;
    }
    send_packet(ch, encode_render());
}

}

int main()
{
    const std::string sock_path = "/tmp/lcd-test.sock";

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

    std::thread accept_thread([&]{ server.accept(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // --- Test client (같은 프로세스 안에서 server 에 connect) ---
    UdsClient client(sock_path);
    auto channel = client.connect();

    std::cout << "[client] connected. press buttons to cycle frames..." << std::endl;

    int frame = 0;
    draw_frame(*channel, frame);   // 초기 화면

    while (true)
    {
        Packet pkt = channel->recv();
        KeyEvent ev;
        std::memcpy(&ev, pkt.data.data(), sizeof(ev));

        if (ev.state != KeyState::Pressed) continue;

        ++frame;
        std::cout << "[client] key=" << static_cast<int>(ev.key)
                  << " -> frame " << (frame % 5) << std::endl;
        draw_frame(*channel, frame);
    }

    accept_thread.join();
    return 0;
}
