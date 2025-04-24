#include "aggregator-positions.h"

namespace {
    using namespace piece::positions;

    color_position_lookup calc_lookup(const color_piece_lookup& pieces);
    board::bitmap::Squares calc_all(const color_position_lookup &lookup);
}


namespace piece::positions
{
    PositionAggregator::PositionAggregator(const color_piece_lookup& pieces) {
        this->positions_of_color = calc_lookup(pieces);
        this->all = calc_all(this->positions_of_color);
    }

    sqr::Squares PositionAggregator::positions() const {
        return this->all;
    }

    sqr::Squares PositionAggregator::positions(Color color) const {
        return this->positions_of_color[static_cast<int>(color)];
    }

}

namespace {
    using namespace piece::positions;

    piece::positions::color_position_lookup calc_lookup(const piece::positions::color_piece_lookup& pieces) {
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

    board::bitmap::Squares calc_all(const piece::positions::color_position_lookup &lookup) {
        sqr::Squares all = 0;

        for (int i = 0; i < static_cast<int>(Color::COUNT); ++i)
        {
            // collect positions of pieces of all color
            all |= lookup[i];
        }

        return all;
    }
}