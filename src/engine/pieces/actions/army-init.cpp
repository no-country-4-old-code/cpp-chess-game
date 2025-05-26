#include <array>
#include "army.h"
#include "board.h"
#include "piece-actions.h"
#include "piece.h"
#include "squares.h"

namespace piece::api {
    void init_army_list(piece::army::army_list &army_list, const board::Board &board) {
        std::array<board::bitmap::Squares, army::max_num_of_armies> army_positions_lookup{};
        board::bitmap::Squares positions_all = 0;

        // update and calc positions
        for (auto i = 0; i < army_list.size(); ++i) {
            army_positions_lookup.at(i) = 0;
            for (auto &current : army_list.at(i).pieces) {
                army_positions_lookup.at(i) |= current.position;
                positions_all |= current.position;
            }
        }

        auto idx_army = 0;
        for (auto &army : army_list) {
            auto positions_hostile_pieces = positions_all & ~army_positions_lookup.at(idx_army);
            ++idx_army;

            for (auto &piece : army.pieces) {
                piece.update_observed_and_attackable(board, positions_all,
                                                     positions_hostile_pieces);
            }
        }
    }
}  // namespace piece::api