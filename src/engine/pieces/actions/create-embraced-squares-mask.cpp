#include <sys/types.h>
#include <bit>
#include <cassert>
#include <cstdlib>
#include "board.h"
#include "piece-actions.h"
#include "squares.h"

namespace
{
    struct Position
    {
        int horizontal;
        int vertical;
    };

    inline u_int8_t get_bit_index(board::bitmap::Squares);
    inline Position map_to_position(board::bitmap::Squares, const board::Board &);
    inline bool is_straight_or_diagonal(const Position &, const Position &);
    inline int sign(int x)
    {
        return static_cast<int>(x > 0) - static_cast<int>(x < 0);
    }
} // namespace

// TODO: might move to board because no pieces involved
namespace piece::utils
{

    sqrs create_embraced_squares_mask(sqrs pos_bitmap1, sqrs pos_bitmap2,
                                      const board::Board &board)
    {
        if ((pos_bitmap1 == 0U) || (pos_bitmap2 == 0U) || pos_bitmap1 == pos_bitmap2)
        {
            return 0;
        }

        const auto pos1 = map_to_position(pos_bitmap1, board);
        const auto pos2 = map_to_position(pos_bitmap2, board);

        const int dx = pos2.horizontal - pos1.horizontal;
        const int dy = pos2.vertical - pos1.vertical;

        if (!is_straight_or_diagonal(pos1, pos2))
        {
            return 0;
        }

        const int step_x = sign(dx);
        const int step_y = sign(dy);

        int x = pos1.horizontal + step_x;
        int y = pos1.vertical + step_y;

        const int width = board.num_of_squares_horizontal;
        sqrs result = 0;

        while (x != pos2.horizontal || y != pos2.vertical)
        {
            result |= sqrs{1ULL << (y * width + x)};
            x += step_x;
            y += step_y;
        }

        return result;
    }
} // namespace piece::utils

namespace
{
    inline u_int8_t get_bit_index(board::bitmap::Squares squares)
    {
        return static_cast<u_int8_t>(std::countr_zero(squares));
    }

    inline Position map_to_position(const board::bitmap::Squares pos, const board::Board &board)
    {
        assert(std::has_single_bit(pos));
        const u_int8_t bit_index = get_bit_index(pos);
        const int h = bit_index % board.num_of_squares_horizontal;
        const int v = bit_index / board.num_of_squares_horizontal;
        return Position{.horizontal = h, .vertical = v};
    }

    inline bool is_straight_or_diagonal(const Position &a, const Position &b)
    {
        const int dx = std::abs(a.horizontal - b.horizontal);
        const int dy = std::abs(a.vertical - b.vertical);
        return dx == 0 || dy == 0 || dx == dy;
    }
} // namespace
