#pragma once
#include "../../../interface/ILcd.hpp"
#include "../../../shared/Screen.hpp"
#include <cstdint>
#include <array>
#include <memory>

class LcdWriter;
class Worker_thread;

namespace 
{
    constexpr int WIDTH  = screen::WIDTH;
    constexpr int HEIGHT = screen::HEIGHT;
    using Frame = std::array<uint16_t, WIDTH * HEIGHT>;
}

class St7789Lcd : public ILcd
{
private:
    std::shared_ptr<LcdWriter>      writer;      
    std::unique_ptr<Worker_thread> thread_pool;

    Frame                           framebuf{};  
    Frame                           scratch_{};


    void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void render_impl(); 

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
