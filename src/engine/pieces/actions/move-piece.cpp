#include <array>
#include <bit>
#include <cassert>
#include "army.h"
#include "board.h"
#include "piece-actions.h"
#include "piece.h"
#include "squares.h"

namespace piece::api {
    void move_piece(const board::bitmap::Squares src,
                    const board::bitmap::Squares dest,
                    const board::Board &board,
                    piece::army::army_list &army_list) {
        assert(std::has_single_bit(src));
        assert(std::has_single_bit(dest));
        assert(dest != src);

        std::array<board::bitmap::Squares, army::max_num_of_armies>
            army_positions_lookup{};
        board::bitmap::Squares positions_all = 0;

        // update and calc positions
        for (auto i = 0; i < army_list.size(); ++i) {
            army_positions_lookup[i] = 0;
            for (auto &current : army_list[i].pieces) {
                if (current.position == src) {
                    current.position = dest;
                }

                else if (current.position == dest) {
                    current.mark_as_dead();
                }

                army_positions_lookup[i] |= current.position;
                positions_all |= current.position;
            }
        }

        // update observed and attackable of all affected pieces
        auto affected_squares = src | dest;

        auto idx_army = 0;
        for (auto &army : army_list) {
            auto positions_hostile_pieces =
                positions_all & ~army_positions_lookup[idx_army];
            ++idx_army;
            for (auto &current : army.pieces) {
                if (((current.observed | current.position) &
                     affected_squares)) {
                    current.update_observed_and_attackable(
                        board, positions_all, positions_hostile_pieces);
                }
            }
        }
    }
}  // namespace piece::api
