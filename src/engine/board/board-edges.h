#include "board-movements.h"
#include <utility>
#include "board.h"
#include "squares.h"

namespace board::edges {
    board::bitmap::Squares build_left_squares_mask(const board::Board &board);
    board::bitmap::Squares build_right_squares_mask(const board::Board &board);
    board::bitmap::Squares build_top_squares_mask(const board::Board &board);
    board::bitmap::Squares build_bottom_squares_mask(const board::Board &board);
}
