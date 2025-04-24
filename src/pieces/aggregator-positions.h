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

    class PositionAggregator
    {
    private:
        sqr::Squares all;
        color_position_lookup positions_of_color;
    public:
        PositionAggregator(const color_piece_lookup& pieces);
        sqr::Squares positions() const;
        sqr::Squares positions(Color color) const;
    };

}
