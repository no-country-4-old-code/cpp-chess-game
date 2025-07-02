#include "board-movements.h"
#include "board.h"
#include "squares.h"
#include "board-edges.h"

namespace board::movements {

    bitmap::Squares left(bitmap::Squares position, const Board &board) {
        auto mask = edges::build_left_squares_mask(board);
        if ((position & mask)) {
            // We are already as 'left' as we can be.
            return 0;
        }
        return position >> 1;
    }

    bitmap::Squares right(bitmap::Squares position, const Board &board) {
        auto mask = edges::build_right_squares_mask(board);
        if ((position & mask)) {
            // We are already as 'right' as we can be.
            return 0;
        }
        return position << 1;
    }

    bitmap::Squares up(bitmap::Squares position, const Board &board) {
        auto mask = edges::build_top_squares_mask(board);
        if ((position & mask)) {
            // We are already as 'up' as we can be.
            return 0;
        }
        return position >> board.num_of_squares_horizontal;
    }

    bitmap::Squares down(bitmap::Squares position, const Board &board) {
        auto mask = edges::build_bottom_squares_mask(board);
        if ((position & mask)) {
            // We are already as 'down' as we can be.
            return 0;
        }
        return position << board.num_of_squares_horizontal;
    }

    // diagnoal movements

    bitmap::Squares left_up(bitmap::Squares position, const Board &board) {
        auto mask = edges::build_left_squares_mask(board);
        mask |= edges::build_top_squares_mask(board);
        if ((position & mask)) {
            return 0;
        }
        return position >> (board.num_of_squares_horizontal + 1);
    }

    bitmap::Squares left_down(bitmap::Squares position, const Board &board) {
        auto mask = edges::build_left_squares_mask(board);
        mask |= edges::build_bottom_squares_mask(board);
        if ((position & mask)) {
            return 0;
        }
        return position << (board.num_of_squares_horizontal - 1);
    }

    bitmap::Squares right_up(bitmap::Squares position, const Board &board) {
        auto mask = edges::build_right_squares_mask(board);
        mask |= edges::build_top_squares_mask(board);
        if ((position & mask)) {
            return 0;
        }
        return position >> (board.num_of_squares_horizontal - 1);
    }

    bitmap::Squares right_down(bitmap::Squares position, const Board &board) {
        auto mask = edges::build_right_squares_mask(board);
        mask |= edges::build_bottom_squares_mask(board);
        if ((position & mask)) {
            return 0;
        }
        return position << (board.num_of_squares_horizontal + 1);
    }

}  // namespace board::movements

