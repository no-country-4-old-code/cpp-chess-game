#include "army.h"
#include "board.h"
#include "iterator-bitfield.h"
#include "piece-actions.h"
#include "piece-type.h"
#include "piece.h"
#include "squares.h"

namespace {
    void handle_castle_move(const piece::army::Army &my_army, const board::Board &board,
                            const piece::api::Context &army_context,
                            piece::api::ArmyDestinations &memory);
}  // namespace

namespace piece::utils {

    void calc_special_moves(const piece::army::Army &my_army, const board::Board &board,
                            const piece::api::Context &context, api::ArmyDestinations &memory) {
        handle_castle_move(my_army, board, context, memory);
    }

}  // namespace piece::utils

namespace {
    void handle_castle_move(const piece::army::Army &my_army, const board::Board &board,
                            const piece::api::Context &context,
                            piece::api::ArmyDestinations &memory) {
        const auto &king_position = my_army.king().position;
        auto tmp                  = king_position & context.under_attack_map;

        if ((!(my_army.king().has_moved())) && (!(king_position & context.under_attack_map))) {

            for (auto& piece: my_army.pieces) {
                if (piece.type == piece::PieceType::ROCK && 
                    (piece.observed & king_position) != 0UL && 
                    !piece.has_moved()) {
                    
                    auto sqrs_between = piece::utils::create_embraced_squares_mask(king_position, piece.position, board);
                    if ((sqrs_between & context.positions_all_armies) == 0UL) {
                        // squares are free

                        board::bitmap::Squares dest_king = 0;
                        board::bitmap::Squares dest_rock = 0;
                        IteratorBitmap iter{sqrs_between};

                        if (king_position > piece.position) {
                            // king is right from rock
                            while (*iter != 0UL) {
                                if (dest_king == 0UL) {
                                    dest_king = *iter;
                                } else if (dest_rock == 0UL) {
                                    dest_rock = *iter;
                                } else {
                                    dest_king = dest_rock;
                                    dest_rock = *iter;
                                }
                                ++iter;
                            }

                        } else {
                            // king is left from rock
                            while (*iter != 0UL) {
                                if (dest_rock == 0UL) {
                                    dest_rock = *iter;
                                } else if (dest_king == 0UL) {
                                    dest_king = *iter;
                                    break;
                                }
                                ++iter;
                            }
                        }

                        if (dest_king && dest_rock) {
                            if (((dest_king | dest_rock) & context.under_attack_map) == 0UL) {
                                // king can move to new dest without threat
                                memory.push({
                                    .src = my_army.king().position, 
                                    .destinations = dest_king,
                                    .extra = {.src=piece.position, .dest=dest_rock}
                                });
                            }
                        }
                    }
                }
            }
        }
    }
}  // namespace