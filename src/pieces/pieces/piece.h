#pragma once
#include "squares.h"
#include "piece-type.h"
#include "board.h"
#include <cassert>
#include <iostream>


namespace piece {

    using sqrs =  board::bitmap::Squares;
    
    class Piece {
        using update_fn = void (*)(piece::Piece& piece, const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies);

        private:
            update_fn update;

        protected:
            Piece(PieceType ptype, board::bitmap::Squares pos, update_fn _fn): update{_fn}, type{ptype}, position{pos} {};
  
        public:
            PieceType type;
            board::bitmap::Squares position;

            board::bitmap::Squares observed; // All "seen" squares and pieces
            board::bitmap::Squares attackable; // All squares the piece might move to (not considering checkmate)
            board::bitmap::Squares movable; // All squares the piece can move to (considering checkmate)

            
            Piece(): Piece(PieceType::_INVALID, 0, [](piece::Piece&, const board::Board&, sqrs, sqrs){}) {};

            void update_observed_and_attackable(const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies) {
                assert(this->type != PieceType::_INVALID);
                update(*this, board, pos_all, pos_hostile_armies);
            };

            void mark_as_dead() {
                position = 0;
                observed = 0;
                attackable = 0;
                movable = 0;
            }

            bool is_alive() const {
                return position > 0;
            }
    };

}
