#pragma once
#include "squares.h"
#include "piece.h"
#include "pieces-color.h"
#include <vector>
#include <array>

namespace piece::positions
{

    namespace sqr = board::bitmap;

    using color_piece_lookup = std::array<std::vector<Piece>, static_cast<size_t>(Color::COUNT)>; // TODO: should be global type
    using color_position_lookup = std::array<sqr::Squares, static_cast<size_t>(Color::COUNT)>;

    struct PositionAggregator
    {
        // Container for positions of all pieces available as ...

        // Do not use const functions here !
        // Make it a class, put all variables into private and add member functions
        // positions() and positions(Color color)
    private:
        sqr::Squares calc_all(color_position_lookup color);
        color_position_lookup calc_lookup(color_piece_lookup pieces);

    public:
        const sqr::Squares all;
        const color_position_lookup color;
        PositionAggregator(color_piece_lookup pieces) : color{calc_lookup(pieces)},
                                                        all{calc_all(color)} {};
    };

}
