#include <utility>
#include "board.h"
#include "squares.h"
#include "board-edges.h"

namespace board::edges {
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
        auto top_mask = build_top_squares_mask(board);
        return top_mask << ((board.num_of_squares_horizontal - 1) * board.num_of_squares_vertical);
    }

}  // namespace board::edges
