#include "piece-api.h"
#include "piece.h"
#include "board.h"
#include "squares.h"
#include "aggregator-positions.h"

namespace {
    void update_movable_pieces_of_army(piece::army::Army army, const board::Board& board, piece::aggregator::army_list& army_list) {
        board::bitmap::Squares under_attack_map = 0x12345; // TODO all attackable from other armies
        board::bitmap::Squares under_observation_map = 0x12345; // TODO all observale from other armies
        
        auto king = army.king();
        bool is_king_under_attack = king.position & under_attack_map;
        board::bitmap::Squares positions_of_my_army = 0x123222; // TODO

        if (is_king_under_attack) {
            int number_of_attackers = 1;

            for (auto i=0; i < army.size(); ++i) {
                army.pieces[i].movable = 0;
            }

            king.movable = king.attackable & ~under_observation_map;
            
            if (number_of_attackers == 1) {
                // only pieces which can intercept the threat to the king !
                piece::Piece* attacker = &army.pieces[1]; //  TODO: find attacker
                // get fields between attacker and king
                board::bitmap::Squares interceptable = 1234;
                for (auto i=0; i < army.size(); ++i) {
                    auto& piece = army.pieces[i];

                    if (piece.position == king.position) {
                        continue;
                    }

                    piece.movable = piece.attackable & interceptable;

                    if (piece.movable) {
                        // Do is under attack check from below
                        // piece.position & under_attack_map
                    }
                }   
                
            }

            

            // pass
        } else {
            for (auto i=0; i < army.size(); ++i) {
                auto& piece = army.pieces[i];

                if (piece.position == king.position) {
                    king.movable = king.attackable & ~under_observation_map;
                }

                else if (piece.position & under_attack_map) {
                    bool does_movement_endanger_king = false;
                    auto positions_without_piece = positions_of_my_army & ~piece.position;
                    // check enemy pieces
                    for (auto& enemy_army: army_list) {
                        if (enemy_army.color() == army.color()) {
                            continue; 
                        }

                        for (auto j=0; j < army.size(); ++j) {
                            auto& enemy = enemy_army.pieces[j];

                            if(piece.position & enemy.attackable) {
                                // add quick check if relative position to each other (if king left to piece then attacker has to be right to piece etc.)
                                
                                // heavy load calcs:
                                auto tmp = enemy;
                                tmp.update_observed_and_attackable(board, positions_without_piece, positions_without_piece);

                                if (king.position & tmp.attackable) {
                                    does_movement_endanger_king = true;
                                    break;
                                }
                            }
                        }

                        if(does_movement_endanger_king) {
                            break;
                        }
                    }
                    
                    if(does_movement_endanger_king) {
                        // movement lead to checkmate
                        piece.movable = 0;
                    } else {
                        // free to move
                        piece.movable = piece.observed;
                    }
                }
                else {
                    // free to move
                    piece.movable = piece.observed;
                }
            }
        }
    }

}

namespace piece::api {
    
    void move_piece(piece::Piece& piece, board::bitmap::Squares dest, const board::Board& board, piece::aggregator::army_list& army_list) {

        // set position of current piece
        auto old_position = piece.position;
 
        // remove old piece // TODO: there is one to remvoe
        for (auto& army : army_list) {
            for (auto i = 0; i < army.size(); ++i) {
                piece::Piece& current = army.pieces[i];

                if (current.position == dest) {
                    current.mark_as_dead();
                }
            }
        }

        piece.position = dest;

        // aggregate positions // TODO: Optimize : Can be done together with the above
        piece::aggregator::PositionAggregator aggr{army_list};
        auto positions_all = aggr.positions();

        // update observed and attackable of all affected pieces
        auto affected_squares = old_position | dest;

        auto idx_army = 0;
        for (auto& army : army_list) {
            auto positions_hostile_pieces = positions_all & ~aggr.positions(idx_army);
            ++idx_army;
            for (u_int8_t i = 0; i < army.size(); ++i) { // TODO: Overwrite iterator of army to use it like piece: army
                auto& current = army.pieces[i];

                if((current.observed | current.position) & affected_squares) {
                    current.update_observed_and_attackable(board, positions_all, positions_hostile_pieces);    
                }
            }
        }

        // update movable for all pieces
        for (auto& army : army_list) {
            for (auto i = 0; i < army.size(); ++i) {
                auto& current = army.pieces[i];
                // TODO: will be implemented later on
                current.movable = current.attackable; 
            }
        }
    }


    void init_army_list(piece::aggregator::army_list& army_list, const board::Board& board) {

        piece::aggregator::PositionAggregator aggr{army_list};
        auto positions_all = aggr.positions();

        auto idx_army = 0;
        for (auto& army : army_list) {
            auto positions_hostile_pieces = positions_all & ~aggr.positions(idx_army);
            ++idx_army;

            for (u_int8_t i = 0; i < army.size(); ++i) {
                army.pieces[i].update_observed_and_attackable(board, positions_all, positions_hostile_pieces);    
            }
        }

        // update movable for all pieces
        for (auto& army : army_list) {
            for (auto i = 0; i < army.size(); ++i) {
                auto& current = army.pieces[i];
                // TODO: will be implemented later on
                current.movable = current.attackable; 
            }
        }
    }
}
