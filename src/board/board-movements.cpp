#include "board-movements.h"
#include "board.h"
#include "squares.h"

namespace {
    board::bitmap::Squares build_left_squares_mask(const board::Board& board);
    board::bitmap::Squares build_right_squares_mask(const board::Board& board);
}

namespace board::movements {

    bitmap::Squares left(bitmap::Squares position, const Board& board) {
        auto mask = build_left_squares_mask(board); // OPTIMIZE: pull in board later on for speed up
        if (position & mask) {
            // We are already as 'left' as we can be.
            return 0;
        } else {
            return position >> 1;
        }
    }

    bitmap::Squares right(bitmap::Squares position, const Board& board) {
        auto mask = build_right_squares_mask(board); // OPTIMIZE: pull in board later on for speed up
        if (position & mask) {
            // We are already as 'right' as we can be.
            return 0;
        } else {
            return position << 1;
        }
    }
}

namespace {
    board::bitmap::Squares build_left_squares_mask(const board::Board& board) {
        // Create a bitmap representing all square on the left edge (a1, a2, a3, ...)
        board::bitmap::Squares mask = 0;
        for (int i = 0; i < board.num_of_squares_vertical; i++) {
            mask |= 1ULL << (board.num_of_squares_horizontal * i);
        }
        return mask;
    }

    board::bitmap::Squares build_right_squares_mask(const board::Board& board) {
        // Create a bitmap representing all square on the left edge (a1, a2, a3, ...)
        board::bitmap::Squares mask = 0;
        board::bitmap::Squares right_side_bit = 1ULL << (board.num_of_squares_horizontal - 1);
        for (int i = 0; i < board.num_of_squares_vertical; i++) {
            mask |= right_side_bit << (board.num_of_squares_horizontal * i);
        }
        return mask;
    }
}
