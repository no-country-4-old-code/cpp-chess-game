#include "aggregator-positions.h"

namespace
{
    using namespace piece::aggregator;

    army_positions_lookup calc_positions_of_army(const army_list &pieces);
    board::bitmap::Squares calc_all(const army_positions_lookup &lookup);
}

namespace piece::aggregator
{
    PositionAggregator::PositionAggregator(const army_list &list)
    {
        this->positions_of_army = calc_positions_of_army(list);
        this->all = calc_all(this->positions_of_army);
    }

    sqr::Squares PositionAggregator::positions() const
    {
        return this->all;
    }

    sqr::Squares PositionAggregator::positions(u_int8_t idx_army) const
    {
        return this->positions_of_army[idx_army];
    }

}

namespace
{
    using namespace piece::aggregator;

    piece::aggregator::army_positions_lookup calc_positions_of_army(const piece::aggregator::army_list &army_list)
    {
        army_positions_lookup lookup;

        for (auto i = 0; i < army_list.size(); ++i)
        {
            lookup[i] = 0;
            for (auto p = 0; p < army_list[i].size(); ++p)
            {
                lookup[i] |= army_list[i].pieces[p].position;
            }
        }

        return lookup;
    }

    board::bitmap::Squares calc_all(const piece::aggregator::army_positions_lookup &lookup)
    {
        sqr::Squares all = 0;

        for (int i = 0; i < lookup.size(); ++i)
        {
            // collect positions of pieces of all armies
            all |= lookup[i];
        }

        return all;
    }
}