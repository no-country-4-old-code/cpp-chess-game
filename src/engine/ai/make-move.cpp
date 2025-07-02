#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>
#include "army.h"
#include "board.h"
#include "chess-ai.h"
#include "notation.h"
#include "piece-actions.h"
#include "run-simulation.h"

namespace {
    inline size_t get_idx(const piece::army::Army &my_army,
                          const piece::army::army_list &army_list);
    inline void execute_move(const ai::simulation::Move &result, piece::army::Army &my_army,
                             piece::army::army_list &army_list, const board::Board &board);
}

namespace ai {
    Move ChessAI::make_move(piece::army::Army &my_army) {
        auto army_index = get_idx(my_army, this->_army_list);
        auto result     = simulation::run_simulation(this->_board, this->_army_list, army_index);
        execute_move(result, my_army, this->_army_list, this->_board);

        return result.move;
    }
}  // namespace ai

namespace {

    inline size_t get_idx(const piece::army::Army &my_army, const piece::army::army_list &army_list)
    {
        size_t army_index = 0;
        for (auto army : army_list) {
            if (army.color() == my_army.color()) {
                break;
            }
            ++army_index;
        }
        return army_index;
    }

    inline void execute_move(const ai::simulation::Move &result, piece::army::Army &my_army,
                             piece::army::army_list &army_list, const board::Board &board) {
        if (result.move.src) {
            piece::api::move_piece(result.move.src, result.move.dest, board, army_list);

            if (result.move.extra.src) {
                piece::api::move_piece(result.move.extra.src, result.move.extra.dest, board,
                                       army_list);
            }

            std::cout << "Moved from " << board::notation::ChessNotation(result.move.src, board)
                      << " to " << board::notation::ChessNotation(result.move.dest, board) << '\n';
        } else {
            my_army.mark_as_defeated();
            std::cout << "# CHECKMATE - No further move possible !!!\n" << std::flush;
        }
    }
}  // namespace