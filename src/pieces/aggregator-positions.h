#pragma once
#include "squares.h"
#include "piece.h"
#include "pieces-color.h"
#include "army.h"
#include <vector>
#include <array>

namespace piece::positions
{

    namespace sqr = board::bitmap;

    using army_list = std::array<piece::army::Army, army::max_num_of_armies>; // TODO: should be global type
    using army_positions_lookup = std::array<sqr::Squares, army::max_num_of_armies>;

    class PositionAggregator
    {
    private:
        sqr::Squares all;
        army_positions_lookup positions_of_army;

    public:
        PositionAggregator(const army_list &pieces);
        sqr::Squares positions() const;
        sqr::Squares positions(u_int8_t idx_army) const;
    };

}
