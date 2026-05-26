#include "Board.hpp"

#include <sdk/App.hpp>
#include <sdk/Draw.hpp>
#include <shared/Screen.hpp>
#include <util/color.hpp>

#include <cstdint>
#include <string>

namespace
{

namespace color = util::color;

constexpr auto FONT_SIZE = util::font::TextSize::Small;

constexpr int CELL     = 24;
constexpr int STATUS_H = 24;
constexpr int BOARD_PX = Board::W * CELL;
constexpr int BOARD_X  = (screen::WIDTH - BOARD_PX) / 2;
constexpr int BOARD_Y  = STATUS_H;

constexpr color::Rgb565 COLOR_BG         = color::BLACK;
constexpr color::Rgb565 COLOR_STATUS_TXT = color::WHITE;
constexpr color::Rgb565 COLOR_HIDDEN     = color::GRAY;
constexpr color::Rgb565 COLOR_OPEN       = color::LIGHT_GRAY;
constexpr color::Rgb565 COLOR_GRID       = color::DARK_GRAY;
constexpr color::Rgb565 COLOR_CURSOR     = color::YELLOW;
constexpr color::Rgb565 COLOR_FLAG       = color::RED;
constexpr color::Rgb565 COLOR_MINE       = color::BLACK;
constexpr color::Rgb565 COLOR_WIN        = color::GREEN;
constexpr color::Rgb565 COLOR_LOSE       = color::RED;

constexpr color::Rgb565 NUM_COLOR[9] = {
    color::BLACK,
    color::BLUE,
    color::DARK_GREEN,
    color::RED,
    color::NAVY,
    color::MAROON,
    color::CYAN,
    color::BLACK,
    color::GRAY,
};

int text_pixel_width(const std::string& s)
{
    return static_cast<int>(s.size()) * util::font::width(FONT_SIZE);
}

}

class MinesweeperApp : public sdk::App
{
public:
    using App::App;

protected:
    void on_draw() override
    {
        draw().clear(COLOR_BG);
        draw_status();
        draw_board();
        draw_overlay();
    }

    void on_key(const KeyEvent& ev) override
    {
        if (ev.state != KeyState::Pressed) return;

        switch (ev.key)
        {
        case Key::Up:     move_cursor(0, -1); break;
        case Key::Down:   move_cursor(0,  1); break;
        case Key::Left:   move_cursor(-1, 0); break;
        case Key::Right:  move_cursor( 1, 0); break;
        case Key::Center: board_.reveal(cursor_x_, cursor_y_);      break;
        case Key::K1:     board_.toggle_flag(cursor_x_, cursor_y_); break;
        case Key::K2:     restart(); break;
        case Key::K4:     App::exit(); return;
        default: break;
        }
    }

private:
    void move_cursor(int dx, int dy)
    {
        int nx = cursor_x_ + dx;
        int ny = cursor_y_ + dy;
        if (nx < 0 || nx >= Board::W) return;
        if (ny < 0 || ny >= Board::H) return;
        cursor_x_ = nx;
        cursor_y_ = ny;
    }

    void restart()
    {
        board_.reset();
        cursor_x_ = Board::W / 2;
        cursor_y_ = Board::H / 2;
    }

    void draw_text_centered(int cx, int cy, const std::string& s, uint16_t color)
    {
        int x = cx - text_pixel_width(s) / 2;
        int y = cy - util::font::height(FONT_SIZE) / 2;
        draw().draw_text(x, y, s, FONT_SIZE, color);
    }

    void draw_status()
    {
        int remaining = board_.mines_total() - board_.flags_placed();
        std::string left = "M:" + std::to_string(remaining);

        std::string right;
        switch (board_.state())
        {
        case Board::State::Ready:   right = "READY"; break;
        case Board::State::Playing: right = "PLAY";  break;
        case Board::State::Won:     right = "WIN";   break;
        case Board::State::Lost:    right = "LOSE";  break;
        }

        const int margin = 4;
        const int ty     = (STATUS_H - util::font::height(FONT_SIZE)) / 2;
        const int rx     = screen::WIDTH - text_pixel_width(right) - margin;

        draw().draw_text(margin, ty, left,  FONT_SIZE, COLOR_STATUS_TXT);
        draw().draw_text(rx,     ty, right, FONT_SIZE, COLOR_STATUS_TXT);
    }

    void draw_board()
    {
        for (int y = 0; y < Board::H; ++y)
            for (int x = 0; x < Board::W; ++x)
                draw_cell(x, y);

        draw_cursor();
    }

    void draw_cell(int cx, int cy)
    {
        const Cell& c = board_.cell(cx, cy);
        int px = BOARD_X + cx * CELL;
        int py = BOARD_Y + cy * CELL;

        uint16_t bg = c.revealed ? COLOR_OPEN : COLOR_HIDDEN;
        draw().fill_rect(px, py, CELL, CELL, bg);
        draw().draw_rect(px, py, CELL, CELL, COLOR_GRID);

        if (c.revealed)
        {
            if (c.has_mine)
            {
                draw().fill_circle(px + CELL / 2, py + CELL / 2, 6, COLOR_MINE);
            }
            else if (c.adjacent > 0)
            {
                std::string s(1, static_cast<char>('0' + c.adjacent));
                draw_text_centered(px + CELL / 2, py + CELL / 2,
                                   s, NUM_COLOR[c.adjacent]);
            }
        }
        else if (c.flagged)
        {
            draw().fill_rect(px + 7, py + 6, 10, 12, COLOR_FLAG);
        }
    }

    void draw_cursor()
    {
        int px = BOARD_X + cursor_x_ * CELL;
        int py = BOARD_Y + cursor_y_ * CELL;
        draw().draw_rect(px,     py,     CELL,     CELL,     COLOR_CURSOR);
        draw().draw_rect(px + 1, py + 1, CELL - 2, CELL - 2, COLOR_CURSOR);
    }

    void draw_overlay()
    {
        if (board_.state() == Board::State::Won)
        {
            draw_text_centered(screen::WIDTH / 2, screen::HEIGHT / 2,
                               "YOU WIN", COLOR_WIN);
        }
        else if (board_.state() == Board::State::Lost)
        {
            draw_text_centered(screen::WIDTH / 2, screen::HEIGHT / 2,
                               "GAME OVER", COLOR_LOSE);
        }
    }

    Board board_;
    int   cursor_x_ = Board::W / 2;
    int   cursor_y_ = Board::H / 2;
};

int main()
{
    MinesweeperApp app;
    app.run();
    return 0;
}
