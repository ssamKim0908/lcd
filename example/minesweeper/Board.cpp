#include "Board.hpp"

#include <cstdlib>
#include <ctime>
#include <vector>

namespace
{
constexpr int DX[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
constexpr int DY[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
}

Board::Board()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    reset();
}

void Board::reset()
{
    for (auto& row : cells_)
        for (auto& c : row)
            c = Cell{};

    state_          = State::Ready;
    flags_          = 0;
    revealed_count_ = 0;
}

bool Board::in_bounds(int x, int y)
{
    return x >= 0 && x < W && y >= 0 && y < H;
}

void Board::place_mines(int safe_x, int safe_y)
{
    auto is_safe = [&](int x, int y) {
        if (x == safe_x && y == safe_y) return true;
        for (int i = 0; i < 8; ++i)
            if (x == safe_x + DX[i] && y == safe_y + DY[i]) return true;
        return false;
    };

    std::vector<int> pool;
    pool.reserve(W * H);
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            if (!is_safe(x, y)) pool.push_back(y * W + x);

    int placed = 0;
    while (placed < MINES && !pool.empty())
    {
        int idx = std::rand() % static_cast<int>(pool.size());
        int p   = pool[idx];
        pool[idx] = pool.back();
        pool.pop_back();

        cells_[p / W][p % W].has_mine = true;
        ++placed;
    }
}

void Board::compute_adjacency()
{
    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            if (cells_[y][x].has_mine) continue;
            int n = 0;
            for (int i = 0; i < 8; ++i)
            {
                int nx = x + DX[i];
                int ny = y + DY[i];
                if (in_bounds(nx, ny) && cells_[ny][nx].has_mine) ++n;
            }
            cells_[y][x].adjacent = static_cast<uint8_t>(n);
        }
    }
}

void Board::flood_fill(int x, int y)
{
    if (!in_bounds(x, y))           return;
    Cell& c = cells_[y][x];
    if (c.revealed || c.flagged)    return;

    c.revealed = true;
    ++revealed_count_;

    if (c.adjacent != 0) return;

    for (int i = 0; i < 8; ++i)
        flood_fill(x + DX[i], y + DY[i]);
}

bool Board::check_win() const
{
    return revealed_count_ == W * H - MINES;
}

void Board::reveal(int x, int y)
{
    if (!in_bounds(x, y))                         return;
    if (state_ == State::Won || state_ == State::Lost) return;

    if (state_ == State::Ready)
    {
        place_mines(x, y);
        compute_adjacency();
        state_ = State::Playing;
    }

    Cell& c = cells_[y][x];
    if (c.revealed || c.flagged) return;

    if (c.has_mine)
    {
        c.revealed = true;
        for (auto& row : cells_)
            for (auto& cc : row)
                if (cc.has_mine) cc.revealed = true;
        state_ = State::Lost;
        return;
    }

    flood_fill(x, y);
    if (check_win()) state_ = State::Won;
}

void Board::toggle_flag(int x, int y)
{
    if (!in_bounds(x, y))                              return;
    if (state_ == State::Won || state_ == State::Lost) return;

    Cell& c = cells_[y][x];
    if (c.revealed) return;

    c.flagged = !c.flagged;
    flags_   += c.flagged ? 1 : -1;
}
