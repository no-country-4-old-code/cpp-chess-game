#pragma once
#include "board.h"
#include "squares.h"

namespace board::movements {
    using move_func = bitmap::Squares (*)(bitmap::Squares position, const Board& board);

    bitmap::Squares left(bitmap::Squares position, const Board& board);
    bitmap::Squares right(bitmap::Squares position, const Board& board);
    bitmap::Squares up(bitmap::Squares position, const Board& board);
    bitmap::Squares down(bitmap::Squares position, const Board& board);

    // diagonal movements
    bitmap::Squares left_up(bitmap::Squares position, const Board& board);
    bitmap::Squares left_down(bitmap::Squares position, const Board& board);
    bitmap::Squares right_up(bitmap::Squares position, const Board& board);
    bitmap::Squares right_down(bitmap::Squares position, const Board& board);
}
