#include <array>
#include "army.h"
#include "board.h"
#include <cstdlib>
#include "piece-actions.h"
#include "piece.h"
#include "squares.h"

namespace
{

    struct Positions
    {
        board::bitmap::Squares positions_all = 0;
        std::array<board::bitmap::Squares, piece::army::max_num_of_armies> army_positions_lookup{};
    };
    Positions collect_positions(const piece::army::army_list &);
    void update_pieces(const Positions &, const board::Board &, piece::army::army_list &);

} // namespace

namespace piece::api
{

    void init_army_list(piece::army::army_list &army_list, const board::Board &board)
    {
        auto positions = collect_positions(army_list);
        update_pieces(positions, board, army_list);
    }

} // namespace piece::api

namespace
{

    Positions collect_positions(const piece::army::army_list &army_list)
    {
        Positions positions;

        for (size_t i = 0; i < army_list.size(); ++i)
        {
            positions.army_positions_lookup.at(i) = 0;
            for (const auto &current : army_list.at(i).pieces)
            {
                positions.army_positions_lookup.at(i) |= current.position;
                positions.positions_all |= current.position;
            }
        }
        return positions;
    }

    void update_pieces(const Positions &positions, const board::Board &board, piece::army::army_list &army_list)
    {
        size_t idx_army = 0;
        for (auto &army : army_list)
        {
            auto positions_hostile_pieces = positions.positions_all & ~positions.army_positions_lookup.at(idx_army);
            ++idx_army;

            for (auto &piece : army.pieces)
            {
                piece.update(board, positions.positions_all, positions_hostile_pieces);
            }
        }
    }
} // namespace