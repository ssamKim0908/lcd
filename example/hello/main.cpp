#include <sdk/App.hpp>
#include <sdk/Draw.hpp>

#include <cstdint>

namespace
{

constexpr uint16_t COLOR_BLACK = 0x0000;
constexpr uint16_t COLOR_WHITE = 0xFFFF;
constexpr uint16_t COLOR_GREEN = 0x07E0;
constexpr uint16_t COLOR_RED   = 0xF800;

constexpr int32_t SCREEN_W   = 240;
constexpr int32_t SCREEN_H   = 240;
constexpr int32_t CURSOR_SZ  = 30;
constexpr int32_t MOVE_STEP  = 10;

}

class HelloApp : public sdk::App
{
public:
    using App::App;

protected:
    void on_draw() override
    {
        draw().clear(COLOR_BLACK);

        draw().draw_text(8, 8, "Hello SDK", util::font::TextSize::Small,
                         COLOR_WHITE);

        draw().fill_rect(x_, y_, CURSOR_SZ, CURSOR_SZ, COLOR_GREEN);
        draw().draw_rect(x_, y_, CURSOR_SZ, CURSOR_SZ, COLOR_WHITE);

        if (centered_)
        {
            draw().fill_circle(SCREEN_W / 2, SCREEN_H / 2, 6, COLOR_RED);
        }
    }

    void on_key(const KeyEvent& ev) override
    {
        if (ev.state != KeyState::Pressed) return;

        switch (ev.key)
        {
        case Key::Up:     y_ -= MOVE_STEP; break;
        case Key::Down:   y_ += MOVE_STEP; break;
        case Key::Left:   x_ -= MOVE_STEP; break;
        case Key::Right:  x_ += MOVE_STEP; break;
        case Key::Center:
            x_ = (SCREEN_W - CURSOR_SZ) / 2;
            y_ = (SCREEN_H - CURSOR_SZ) / 2;
            centered_ = !centered_;
            break;
        case Key::K4:
            App::exit();
            return;
        default: break;
        }

        if (x_ < 0)                        x_ = 0;
        if (y_ < 0)                        y_ = 0;
        if (x_ > SCREEN_W - CURSOR_SZ)     x_ = SCREEN_W - CURSOR_SZ;
        if (y_ > SCREEN_H - CURSOR_SZ)     y_ = SCREEN_H - CURSOR_SZ;
    }

private:
    int32_t x_        = (SCREEN_W - CURSOR_SZ) / 2;
    int32_t y_        = (SCREEN_H - CURSOR_SZ) / 2;
    bool    centered_ = true;
};

int main()
{
    HelloApp app;
    app.run();
    return 0;
}
