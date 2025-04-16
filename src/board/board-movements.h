#pragma once
#include "board.h"
#include "squares.h"

namespace board::movements {
    bitmap::Squares left(bitmap::Squares position, const Board& board);
    bitmap::Squares right(bitmap::Squares position, const Board& board);
    bitmap::Squares up(bitmap::Squares position, const Board& board);
    bitmap::Squares down(bitmap::Squares position, const Board& board);
}
