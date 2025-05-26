#include <utility>
#include "army.h"

#include "board.h"
#include "piece-actions.h"
#include "piece.h"
#include "squares.h"

namespace {

    struct Context {
        board::bitmap::Squares position_my_army;
        board::bitmap::Squares under_attack_map;
    };

    bool does_piece_movement_endanger_own_king(const piece::Piece &, const board::Board &,
                                               const piece::army::army_list &,
                                               const piece::army::Army &,
                                               const Context&);
} // namespace

namespace piece::api {

    ArmyDestinations calc_possible_moves(const piece::army::Army &my_army,
                                         const board::Board &board,
                                         const piece::army::army_list &army_list) {
        board::bitmap::Squares enemy_attack_map      = 0;
        board::bitmap::Squares enemy_observation_map = 0;
        board::bitmap::Squares my_positions_map      = 0;
        unsigned int number_of_king_attackers        = 0;
        const piece::Piece *king_attacker            = nullptr;
        const auto &king                             = my_army.king();
        ArmyDestinations memory;

        for (const auto &army : army_list) {
            if (army.color() == my_army.color()) {
                for (const auto &piece : army.pieces) {
                    my_positions_map |= piece.position;
                }
            } else {
                for (const auto &piece : army.pieces) {
                    enemy_attack_map |= piece.attackable;
                    enemy_observation_map |= piece.observed;

                    if ((piece.attackable & king.position)) {
                        ++number_of_king_attackers;
                        king_attacker = &piece;
                    }
                }
            }
        }

        // king movement
        auto movable_king = king.attackable & ~enemy_observation_map;
        if (movable_king) {
            memory.push({.src = king.position, .destinations = movable_king});
        }

        // other pieces
        if (number_of_king_attackers == 0) {
            for (const auto &piece : my_army.pieces) {
                if (piece.position == king.position) {
                    continue;
                }
                if ((piece.position & enemy_attack_map)) {
                    bool const result = does_piece_movement_endanger_own_king(
                        piece, board, army_list, my_army, 
                        Context{
                            .position_my_army = my_positions_map,
                            .under_attack_map = enemy_attack_map
                        });

                    if (!result) {
                        memory.push({.src = piece.position, .destinations = piece.attackable});
                    }
                } else {
                    memory.push({.src = piece.position, .destinations = piece.attackable});
                }
            }
        } else if (number_of_king_attackers == 1) {
            // get fields between attacker and king
            auto interceptable =
                utils::create_embraced_squares_mask(king_attacker->position, king.position, board);
            interceptable |= king_attacker->position;

            for (const auto &piece : my_army.pieces) {
                if (piece.position == king.position) {
                    continue;
                }

                auto movable = piece.attackable & interceptable;

                if (movable) {
                    bool const result = does_piece_movement_endanger_own_king(
                        piece, board, army_list, my_army,
                        Context{
                            .position_my_army = my_positions_map,
                            .under_attack_map = enemy_attack_map
                        });
                    if (!result) {
                        memory.push({.src = piece.position, .destinations = movable});
                    }
                }
            }
        }
        return memory;
    }
}  // namespace piece::api


namespace {
    bool does_piece_movement_endanger_own_king(const piece::Piece &piece, const board::Board &board,
                                               const piece::army::army_list &army_list,
                                               const piece::army::Army &my_army,
                                               const Context& context) {

        if ((piece.position & context.under_attack_map))  // only matter if piece is under attack at all
        {
            auto king_position           = my_army.king().position;
            auto positions_without_piece = context.position_my_army & ~piece.position;
            // check enemy pieces
            for (const auto &enemy_army : army_list) {
                if (enemy_army.color() == my_army.color()) {
                    continue;
                }

                for (auto j = 0; std::cmp_less(j, enemy_army.size()); ++j) {
                    const auto &enemy = enemy_army.pieces[j];

                    if ((piece.position & enemy.attackable)) {
                        if ((king_position & enemy.attackable)) {
                            // if king is already attacked by this piece, then
                            // movement of piece does not matter
                            continue;
                        }
                        // (optimize) add quick check if relative position to
                        // each other (if king left to piece then attacker has
                        // to be right to piece etc.)

                        // heavy load calcs:
                        auto tmp = enemy;
                        tmp.update_observed_and_attackable(board, positions_without_piece,
                                                           positions_without_piece);

                        if ((king_position & tmp.attackable)) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

}  // namespace