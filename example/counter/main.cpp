#include <sdk/App.hpp>
#include <sdk/Draw.hpp>

#include <cstdint>
#include <string>

namespace
{

constexpr uint16_t COLOR_BG    = 0x0000;
constexpr uint16_t COLOR_TITLE = 0x07FF;
constexpr uint16_t COLOR_VALUE = 0xFFFF;
constexpr uint16_t COLOR_HINT  = 0x7BEF;

constexpr int32_t TITLE_X     = 8;
constexpr int32_t TITLE_Y     = 8;
constexpr int32_t VALUE_X     = 60;
constexpr int32_t VALUE_Y     = 100;
constexpr int32_t HINT_X      = 8;
constexpr int32_t HINT_Y_BASE = 180;
constexpr int32_t HINT_LH     = 16;

}

class CounterApp : public sdk::App
{
public:
    using App::App;

protected:
    void on_draw() override
    {
        draw().clear(COLOR_BG);
        draw().draw_text(TITLE_X, TITLE_Y, "Counter",
                         util::font::TextSize::Small, COLOR_TITLE);

        draw().draw_text(VALUE_X, VALUE_Y, std::to_string(value_),
                         util::font::TextSize::Small, COLOR_VALUE);

        draw().draw_text(HINT_X, HINT_Y_BASE + HINT_LH * 0,
                         "L/R: -1/+1", util::font::TextSize::Small,
                         COLOR_HINT);
        draw().draw_text(HINT_X, HINT_Y_BASE + HINT_LH * 1,
                         "U/D: +10/-10", util::font::TextSize::Small,
                         COLOR_HINT);
        draw().draw_text(HINT_X, HINT_Y_BASE + HINT_LH * 2,
                         "K4: exit", util::font::TextSize::Small,
                         COLOR_HINT);
    }

    void on_key(const KeyEvent& ev) override
    {
        if (ev.state != KeyState::Pressed) return;

        switch (ev.key)
        {
        case Key::Right: value_ +=  1; break;
        case Key::Left:  value_ -=  1; break;
        case Key::Up:    value_ += 10; break;
        case Key::Down:  value_ -= 10; break;
        case Key::K4:    App::exit(); return;
        default: break;
        }
    }

private:
    int32_t value_ = 0;
};

int main()
{
    CounterApp app;
    app.run();
    return 0;
}
