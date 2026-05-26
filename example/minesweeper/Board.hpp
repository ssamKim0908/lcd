#pragma once
#include "Cell.hpp"

#include <array>
#include <cstdint>

class Board
{
public:
    static constexpr int W      = 9;
    static constexpr int H      = 9;
    static constexpr int MINES  = 10;

    enum class State : uint8_t { Ready, Playing, Won, Lost };

    Board();

    void reset();

    const Cell& cell(int x, int y) const { return cells_[y][x]; }
    State       state()             const { return state_; }
    int         flags_placed()      const { return flags_; }
    int         mines_total()       const { return MINES; }

    void reveal(int x, int y);
    void toggle_flag(int x, int y);

private:
    void place_mines(int safe_x, int safe_y);
    void compute_adjacency();
    void flood_fill(int x, int y);
    bool check_win() const;
    static bool in_bounds(int x, int y);

    std::array<std::array<Cell, W>, H> cells_{};
    State state_          = State::Ready;
    int   flags_          = 0;
    int   revealed_count_ = 0;
};
