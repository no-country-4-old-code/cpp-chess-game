#include "board-movements.h"
#include "board.h"
#include "squares.h"

namespace board::movements {

    bitmap::Squares left(bitmap::Squares position, const Board& /*board*/) {
        return position;
    }
}