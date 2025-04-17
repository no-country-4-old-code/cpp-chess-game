#include "all-piece-positions.h"

namespace piece::positions
{

    color_position_lookup piece::positions::PositionAggregator::calc_lookup(color_piece_lookup pieces)
    {
        color_position_lookup lookup;

        for (int i = 0; i < static_cast<int>(Color::COUNT); ++i)
        {
            lookup[i] = 0;
            for (auto p : pieces[i])
            {
                lookup[i] |= p.position;
            }
        }

        return lookup;
    }

    sqr::Squares calc_all(const color_position_lookup &lookup)
    {
        sqr::Squares all = 0;

        for (int i = 0; i < static_cast<int>(Color::COUNT); ++i)
        {
            // collect positions of pieces of all color
            all |= lookup[i];
        }

        return all;
    }
}