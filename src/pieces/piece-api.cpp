#pragma once
#include "piece-api.h"
#include "piece.h"
#include "board.h"
#include "squares.h"
#include "aggregator-positions.h"

namespace piece::api {
    
    void move_piece(piece::Piece& piece, board::bitmap::Squares dest, const board::Board& board, piece::aggregator::army_list& army_list) {

        // set position of current piece
        auto old_position = piece.position;
 
        // remove old piece
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

        for (auto& army : army_list) {
            for (u_int8_t i = 0; i < army.size(); ++i) { // TODO: Overwrite iterator of army to use it like piece: army
                auto& current = army.pieces[i];

                if((current.observed | current.position) & affected_squares) {
                    auto positions_hostile_pieces = positions_all & ~aggr.positions(i);
                    piece.update_observed_and_attackable(board, positions_all, positions_hostile_pieces);    
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
}
