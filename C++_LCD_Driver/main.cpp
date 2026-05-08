#include "framework/Ipcserver.hpp"
#include "framework/Rasterizer.hpp"
#include "ipc/UdsServer.hpp"
#include "ipc/UdsClient.hpp"
#include "display/St7789Lcd.hpp"
#include "display/LcdWriter.hpp"
#include "interface/IChannel.hpp"
#include "interface/IKeys.hpp"
#include "shared/Packet.hpp"
#include "shared/RecvResult.hpp"
#include "shared/DrawCommand.hpp"
#include "shared/Paths.hpp"
#include "hal/epoll.hpp"
#include "hal/gpio.hpp"
#include "hal/spi.hpp"
#include "input/GpioKeys.hpp"
#include "util/span.hpp"
#include "util/Font.hpp"
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

std::vector<std::byte> encode_draw_rect(int32_t x, int32_t y,
                                        int32_t w, int32_t h,
                                        uint16_t color)
{
    std::vector<std::byte> v;
    put(v, static_cast<uint8_t>(shared::DrawCommand::DrawRect));
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

std::vector<std::byte> encode_draw_circle(int32_t cx, int32_t cy, int32_t r,
                                          uint16_t color)
{
    std::vector<std::byte> v;
    put(v, static_cast<uint8_t>(shared::DrawCommand::DrawCircle));
    put(v, cx); put(v, cy); put(v, r);
    put(v, color);
    return v;
}

std::vector<std::byte> encode_draw_text(int32_t x, int32_t y,
                                        util::font::TextSize size,
                                        uint16_t color,
                                        const std::string& text)
{
    std::vector<std::byte> v;
    put(v, static_cast<uint8_t>(shared::DrawCommand::DrawText));
    put(v, x); put(v, y);
    put(v, static_cast<uint8_t>(size));
    put(v, color);
    put(v, static_cast<uint16_t>(text.size()));
    const auto* p = reinterpret_cast<const std::byte*>(text.data());
    v.insert(v.end(), p, p + text.size());
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
    switch (frame % 8)
    {
    case 0: // Clear
        send_packet(ch, encode_clear(COLOR_RED));
        break;
    case 1: // FillRect
        send_packet(ch, encode_clear(COLOR_BLACK));
        send_packet(ch, encode_fill_rect(60, 60, 120, 120, COLOR_WHITE));
        break;
    case 2: // DrawRect (테두리만)
        send_packet(ch, encode_clear(COLOR_BLACK));
        send_packet(ch, encode_draw_rect(40, 40, 160, 160, COLOR_GREEN));
        send_packet(ch, encode_draw_rect(80, 80,  80,  80, COLOR_YELLOW));
        break;
    case 3: // FillCircle
        send_packet(ch, encode_clear(COLOR_BLACK));
        send_packet(ch, encode_fill_circle(120, 120, 80, COLOR_YELLOW));
        break;
    case 4: // DrawCircle (테두리만)
        send_packet(ch, encode_clear(COLOR_BLACK));
        send_packet(ch, encode_draw_circle(120, 120, 100, COLOR_CYAN));
        send_packet(ch, encode_draw_circle(120, 120,  60, COLOR_RED));
        break;
    case 5: // DrawText
        send_packet(ch, encode_clear(COLOR_BLACK));
        send_packet(ch, encode_draw_text(20,  40,
                                         util::font::TextSize::Small,
                                         COLOR_WHITE, "Hello, LCD!"));
        send_packet(ch, encode_draw_text(20,  80,
                                         util::font::TextSize::Small,
                                         COLOR_GREEN, "Frame 5"));
        send_packet(ch, encode_draw_text(20, 120,
                                         util::font::TextSize::Small,
                                         COLOR_YELLOW, "0123456789"));
        break;
    case 6: // 모든 도형 조합
        send_packet(ch, encode_clear(COLOR_BLUE));
        send_packet(ch, encode_fill_rect (40, 40, 160, 160, COLOR_BLACK));
        send_packet(ch, encode_draw_rect (40, 40, 160, 160, COLOR_WHITE));
        send_packet(ch, encode_fill_circle(120, 120, 50, COLOR_RED));
        send_packet(ch, encode_draw_circle(120, 120, 70, COLOR_YELLOW));
        send_packet(ch, encode_draw_text(60, 200,
                                         util::font::TextSize::Small,
                                         COLOR_WHITE, "all-in-one"));
        break;
    case 7: // 경계 케이스: 화면 끝, r=0/1, 빈 문자열
        send_packet(ch, encode_clear(COLOR_CYAN));
        send_packet(ch, encode_fill_rect (  0,   0,   1,   1, COLOR_RED));
        send_packet(ch, encode_fill_rect (239, 239,   1,   1, COLOR_RED));
        send_packet(ch, encode_draw_rect (  0,   0, 240, 240, COLOR_BLACK));
        send_packet(ch, encode_fill_circle(120, 120,   1, COLOR_BLACK));
        send_packet(ch, encode_draw_circle(120, 120, 119, COLOR_BLACK));
        send_packet(ch, encode_draw_text(  4,   4,
                                         util::font::TextSize::Small,
                                         COLOR_BLACK, "edge"));
        break;
    }
    send_packet(ch, encode_render());
}

}

int main()
{
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
        RecvResult r = channel->recv();
        if (r.status == RecvResult::Status::Closed) break;
        KeyEvent ev;
        std::memcpy(&ev, r.packet.data.data(), sizeof(ev));

        if (ev.state != KeyState::Pressed) continue;

        ++frame;
        std::cout << "[client] key=" << static_cast<int>(ev.key)
                  << " -> frame " << (frame % 8) << std::endl;
        draw_frame(*channel, frame);
    }

    accept_thread.join();
    return 0;
}
