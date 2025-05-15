#include "piece-embraced-squares-mask.h"
#include <cstdlib>
#include <bit>

namespace {
    struct Position {
       const int horizontal;
       const int vertical;
       Position(int h, int v): horizontal{h}, vertical{v} {};
    };

    inline u_int8_t get_bit_index(board::bitmap::Squares);
    inline Position map_to_position(board::bitmap::Squares,const board::Board&);
}

namespace piece::utils
{
    
    sqrs piece::utils::create_embraced_squares_mask(sqrs pos_bitmap1, sqrs pos_bitmap2, const board::Board& board)
    {
        auto pos1 = map_to_position(pos_bitmap1, board);
        auto pos2 = map_to_position(pos_bitmap2, board);
        
        auto height = std::abs(pos1.vertical - pos2.vertical);
        auto widht = std::abs(pos1.horizontal - pos2.horizontal);

        sqrs mask = ~(-1 << (widht-1)); // fill first "widht" bits
        for (auto i = 1; i < (height-1); ++i) {
            mask |= mask << board.num_of_squares_horizontal; 
        }

        auto offset = (std::min(pos1.horizontal, pos2.horizontal) + 1) * board.num_of_squares_horizontal + (std::min(pos1.vertical, pos1.vertical) + 1);

        return mask << offset;
    }

    

}

namespace {
    inline u_int8_t get_bit_index(board::bitmap::Squares squares) {
        return static_cast<u_int8_t>(std::countr_zero(squares));
    };

    inline Position map_to_position(const board::bitmap::Squares pos, const board::Board& board) {
        assert(std::has_single_bit(pos));
        const u_int8_t bit_index  = get_bit_index(pos);
        const u_int8_t horizontal = bit_index % board.num_of_squares_horizontal;
        const u_int8_t vertical   = bit_index / board.num_of_squares_horizontal;
        return Position(horizontal, vertical);
    }
}
