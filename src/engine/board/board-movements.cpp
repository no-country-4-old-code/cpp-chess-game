#include "board-movements.h"

#include <utility>
#include "board.h"
#include "squares.h"

namespace {
    board::bitmap::Squares build_left_squares_mask(const board::Board &board);
    board::bitmap::Squares build_right_squares_mask(const board::Board &board);
    board::bitmap::Squares build_top_squares_mask(const board::Board &board);
    board::bitmap::Squares build_bottom_squares_mask(const board::Board &board);
}

namespace board::movements {

    bitmap::Squares left(bitmap::Squares position, const Board &board) {
        auto mask =
            build_left_squares_mask(board);  // OPTIMIZE: pull in board later on for speed up
        if ((position & mask)) {
            // We are already as 'left' as we can be.
            return 0;
        }
        return position >> 1;
    }

    bitmap::Squares right(bitmap::Squares position, const Board &board) {
        auto mask =
            build_right_squares_mask(board);  // OPTIMIZE: pull in board later on for speed up
        if ((position & mask)) {
            // We are already as 'right' as we can be.
            return 0;
        }
        return position << 1;
    }

    bitmap::Squares up(bitmap::Squares position, const Board &board) {
        auto mask = build_top_squares_mask(board);  // OPTIMIZE: pull in board later on for speed up
        if ((position & mask)) {
            // We are already as 'up' as we can be.
            return 0;
        }
        return position >> board.num_of_squares_horizontal;
    }

    bitmap::Squares down(bitmap::Squares position, const Board &board) {
        auto mask =
            build_bottom_squares_mask(board);  // OPTIMIZE: pull in board later on for speed up
        if ((position & mask)) {
            // We are already as 'down' as we can be.
            return 0;
        }
        return position << board.num_of_squares_horizontal;
    }

    // diagnoal movements

    bitmap::Squares left_up(bitmap::Squares position, const Board &board) {
        auto mask = build_left_squares_mask(board);
        mask |= build_top_squares_mask(board);
        if ((position & mask)) {
            return 0;
        }
        return position >> (board.num_of_squares_horizontal + 1);
    }

    bitmap::Squares left_down(bitmap::Squares position, const Board &board) {
        auto mask = build_left_squares_mask(board);
        mask |= build_bottom_squares_mask(board);
        if ((position & mask)) {
            return 0;
        }
        return position << (board.num_of_squares_horizontal - 1);
    }

    bitmap::Squares right_up(bitmap::Squares position, const Board &board) {
        auto mask = build_right_squares_mask(board);
        mask |= build_top_squares_mask(board);
        if ((position & mask)) {
            return 0;
        }
        return position >> (board.num_of_squares_horizontal - 1);
    }

    bitmap::Squares right_down(bitmap::Squares position, const Board &board) {
        auto mask = build_right_squares_mask(board);
        mask |= build_bottom_squares_mask(board);
        if ((position & mask)) {
            return 0;
        }
        return position << (board.num_of_squares_horizontal + 1);
    }

}  // namespace board::movements

namespace {
    board::bitmap::Squares build_left_squares_mask(const board::Board &board) {
        // Create a bitmap representing all square on the left edge (a1, a2, a3,
        // ...)
        board::bitmap::Squares mask = 0;
        for (int i = 0; std::cmp_less(i, board.num_of_squares_vertical); i++) {
            mask |= 1ULL << (board.num_of_squares_horizontal * i);
        }
        return mask;
    }

    board::bitmap::Squares build_right_squares_mask(const board::Board &board) {
        // Create a bitmap representing all square on the right edge (e.g. h1,
        // h2, h3, ...)
        board::bitmap::Squares mask                 = 0;
        board::bitmap::Squares const right_side_bit = 1ULL << (board.num_of_squares_horizontal - 1);
        for (int i = 0; std::cmp_less(i, board.num_of_squares_vertical); i++) {
            mask |= right_side_bit << (board.num_of_squares_horizontal * i);
        }
        return mask;
    }

    board::bitmap::Squares build_top_squares_mask(const board::Board &board) {
        // Create a bitmap representing all square on the upper edge (a1, b1,
        // c1, ...)
        return (1ULL << board.num_of_squares_vertical) - 1;
    }

    board::bitmap::Squares build_bottom_squares_mask(const board::Board &board) {
        // Create a bitmap representing all square on the bottom edge (e.g. a8,
        // b8, c8, ...)
        return ~(~0ULL >> board.num_of_squares_vertical);
    }

}  // namespace
