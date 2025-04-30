#include "piece-api.h"
#include "piece.h"
#include "board.h"
#include "squares.h"
#include "aggregator-positions.h"

namespace {


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
