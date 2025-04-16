#pragma once
#include "squares.h"
#include "board.h"

namespace board::movements {
    Board b{9, 9};

    bitmap::Squares left(bitmap::Squares position, const Board& board);
    

}
