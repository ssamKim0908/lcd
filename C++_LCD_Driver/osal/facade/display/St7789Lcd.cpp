#include "St7789Lcd.hpp"
#include "LcdWriter.hpp"
#include "../../../util/span.hpp"
#include "../../../util/time.hpp"
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <utility>

// ====================================================================
//  Presenter
// ====================================================================

Presenter::Presenter(std::shared_ptr<LcdWriter> writer)
    : writer_(std::move(writer))
{
    worker_ = std::thread(&Presenter::render_loop, this);
}

Presenter::~Presenter()
{
    {
        std::lock_guard<std::mutex> lk(mtx_);
        stop_ = true;
    }
    cv_.notify_one();
    if (worker_.joinable()) worker_.join();
}

void Presenter::render(const Frame& framebuf)
{
    std::unique_lock<std::mutex> lk(mtx_);
    cv_.wait(lk, [this] { return !rendering_; });   // 이전 전송 끝까지 대기 (drop 없음)

    scratch_ = framebuf;                            // 스냅샷
    rendering_ = true;
    cv_.notify_one();                               // 워커 깨움
}

void Presenter::set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // MADCTL=0xA0 (MY=1, MV=1): 패널이 RAM row 80..319 영역을 본다
    constexpr uint16_t X_OFFSET = 80;
    constexpr uint16_t Y_OFFSET = 0;

    uint16_t xs = x0 + X_OFFSET;
    uint16_t xe = (x1 - 1) + X_OFFSET;
    uint16_t ys = y0 + Y_OFFSET;
    uint16_t ye = (y1 - 1) + Y_OFFSET;

    writer_->write_cmd(std::byte{0x2A});
    std::byte col[] = {
        std::byte((xs >> 8) & 0xFF), std::byte(xs & 0xFF),
        std::byte((xe >> 8) & 0xFF), std::byte(xe & 0xFF),
    };
    writer_->write_data({col, 4});

    writer_->write_cmd(std::byte{0x2B});
    std::byte row[] = {
        std::byte((ys >> 8) & 0xFF), std::byte(ys & 0xFF),
        std::byte((ye >> 8) & 0xFF), std::byte(ye & 0xFF),
    };
    writer_->write_data({row, 4});

    writer_->write_cmd(std::byte{0x2C});
}

void Presenter::blast()
{
    set_window(0, 0, WIDTH, HEIGHT);

    constexpr size_t CHUNK = 4096;
    util::Span<const std::byte> bytes =
        util::as_bytes(util::Span<const uint16_t>(scratch_.data(), scratch_.size()));

    for (size_t offset = 0; offset < bytes.size(); offset += CHUNK)
    {
        const size_t n = std::min(CHUNK, bytes.size() - offset);
        writer_->write_data({bytes.data() + offset, n});
    }
}

void Presenter::render_loop()
{
    std::unique_lock<std::mutex> lk(mtx_);
    for (;;)
    {
        cv_.wait(lk, [this] { return rendering_ || stop_; });
        if (stop_) return;

        lk.unlock();
        blast();                 // SPI 전송 (락 없이)
        lk.lock();

        rendering_ = false;
        cv_.notify_one();        // render() 에서 대기 중인 메인 깨움
    }
}

// ====================================================================
//  St7789Lcd
// ====================================================================

St7789Lcd::St7789Lcd(std::unique_ptr<LcdWriter> writer)
    : writer(std::move(writer))
{
    presenter = std::make_unique<Presenter>(this->writer);
}

St7789Lcd::~St7789Lcd() = default;

void St7789Lcd::init()
{
    using namespace util::time;

    writer->reset();

    auto cmd  = [this](uint8_t b) { writer->write_cmd(std::byte{b}); };
    auto data = [this](std::initializer_list<uint8_t> bs) {
        std::vector<std::byte> buf;
        buf.reserve(bs.size());
        for (uint8_t b : bs) buf.push_back(std::byte{b});
        writer->write_data({buf.data(), buf.size()});
    };

    cmd(0x11);                                          // sleep out
    sleep_ms(1000);

    cmd(0x36); data({0xA0});                            // MADCTL: MY=0,MX=1,MV=1,ML=0,RGB
    sleep_ms(50);
    cmd(0x3A); data({0x05});                            // 65k colors

    cmd(0xB2); data({0x0C, 0x0C, 0x00, 0x33, 0x33});    // PORCTRL
    cmd(0xB7); data({0x35});                            // GCTRL
    cmd(0xBB); data({0x37});                            // VCOMS
    cmd(0xC0); data({0x2C});                            // LCMCTRL
    cmd(0xC2); data({0x01});                            // VDVVRHEN
    cmd(0xC3); data({0x12});                            // VRHS
    cmd(0xC4); data({0x20});                            // VDVS
    cmd(0xC6); data({0x0F});                            // FRCTRL2 (60Hz)
    cmd(0xD0); data({0xA4, 0xA1});                      // PWCTRL1

    cmd(0xE0); data({0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F,
                     0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23});
    cmd(0xE1); data({0xD4, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F,
                     0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23});

    cmd(0x21);                                          // INVON
    sleep_ms(120);
    cmd(0x29);                                          // DISPON
    sleep_ms(50);
}

void St7789Lcd::clear(uint16_t color)
{
    uint16_t be = static_cast<uint16_t>((color >> 8) | (color << 8));
    framebuf.fill(be);
}

void St7789Lcd::draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= WIDTH || y >= HEIGHT) return;
    uint16_t be = static_cast<uint16_t>((color >> 8) | (color << 8));
    framebuf[y * WIDTH + x] = be;
}

void St7789Lcd::render()
{
    presenter->render(framebuf);
}
