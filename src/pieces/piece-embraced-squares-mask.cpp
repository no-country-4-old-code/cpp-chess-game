#include "piece-embraced-squares-mask.h"
#include <cstdlib>
#include <bit>
#include <cassert>

namespace {
    struct Position {
        const int horizontal;
        const int vertical;
        Position(int h, int v) : horizontal{h}, vertical{v} {};
    };

    inline u_int8_t get_bit_index(board::bitmap::Squares);
    inline Position map_to_position(board::bitmap::Squares, const board::Board&);
    inline bool is_straight_or_diagonal(const Position&, const Position&);
}

namespace piece::utils {

    sqrs create_embraced_squares_mask(sqrs pos_bitmap1, sqrs pos_bitmap2, const board::Board& board) {
        if (!pos_bitmap1 || !pos_bitmap2 || pos_bitmap1 == pos_bitmap2)
            return 0;

        auto pos1 = map_to_position(pos_bitmap1, board);
        auto pos2 = map_to_position(pos_bitmap2, board);

        int dx = pos2.horizontal - pos1.horizontal;
        int dy = pos2.vertical - pos1.vertical;

        if (!is_straight_or_diagonal(pos1, pos2))
            return 0;

        int step_x = (dx == 0 ? 0 : dx / std::abs(dx));
        int step_y = (dy == 0 ? 0 : dy / std::abs(dy));

        sqrs result = 0;

        int x = pos1.horizontal + step_x;
        int y = pos1.vertical + step_y;

        while (x != pos2.horizontal || y != pos2.vertical) {
            int index = y * board.num_of_squares_horizontal + x;
            result |= sqrs{1ULL << index};
            x += step_x;
            y += step_y;
        }

        return result;
    }
}

namespace {
    inline u_int8_t get_bit_index(board::bitmap::Squares squares) {
        return static_cast<u_int8_t>(std::countr_zero(squares));
    }

    inline Position map_to_position(const board::bitmap::Squares pos, const board::Board& board) {
        assert(std::has_single_bit(pos));
        const u_int8_t bit_index = get_bit_index(pos);
        const u_int8_t horizontal = bit_index % board.num_of_squares_horizontal;
        const u_int8_t vertical = bit_index / board.num_of_squares_horizontal;
        return Position(horizontal, vertical);
    }

    inline bool is_straight_or_diagonal(const Position& a, const Position& b) {
        int dx = std::abs(a.horizontal - b.horizontal);
        int dy = std::abs(a.vertical - b.vertical);
        return dx == 0 || dy == 0 || dx == dy;
    }
}
