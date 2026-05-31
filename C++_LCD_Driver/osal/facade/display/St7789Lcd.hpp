#pragma once
#include "../../../interface/ILcd.hpp"
#include <cstdint>
#include <array>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>

class LcdWriter;

namespace 
{
    static constexpr uint16_t WIDTH  = 240;
    static constexpr uint16_t HEIGHT = 240;
    using Frame = std::array<uint16_t, WIDTH * HEIGHT>;
}

class Presenter
{
public:
    Presenter(std::shared_ptr<LcdWriter> writer);
    ~Presenter();

    Presenter(const Presenter&)            = delete;
    Presenter& operator=(const Presenter&) = delete;

    void render(const Frame& framebuf);

private:
    void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void blast();            // scratch_ 를 SPI 로 전송
    void render_loop();      // 워커 본체

    std::shared_ptr<LcdWriter> writer_;     // St7789Lcd 와 공동 소유
    Frame                      scratch_{};  // 전송용 사본

    std::mutex                 mtx_;
    std::condition_variable    cv_;
    bool                       rendering_ = false;  // 전송 진행 중
    bool                       stop_      = false;  // 종료 신호
    std::thread                worker_;
};

class St7789Lcd : public ILcd
{
private:
    std::shared_ptr<LcdWriter>  writer;      // Presenter 와 공동 소유
    std::unique_ptr<Presenter>  presenter;
    Frame                       framebuf{};  // 메인 전용 그리기 버퍼 (락 불필요)

public:
    St7789Lcd(std::unique_ptr<LcdWriter> writer);
    ~St7789Lcd() override;

    St7789Lcd(const St7789Lcd&)            = delete;
    St7789Lcd& operator=(const St7789Lcd&) = delete;

    void init       ()                                       override;
    void clear      (uint16_t color)                         override;
    void draw_pixel (uint16_t x, uint16_t y, uint16_t color) override;
    void render     ()                                       override;
};
