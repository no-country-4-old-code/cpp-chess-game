#pragma once
#include "squares.h"
#include "piece-type.h"
#include "board.h"
#include <cassert>
#include <iostream>


namespace piece {

    using sqrs =  board::bitmap::Squares;
    
    class Piece {
        public:
            using update_fn = void (*)(piece::Piece& piece, const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies);

        private:
            update_fn update;

        public:
            Piece(PieceType ptype, board::bitmap::Squares pos, update_fn _fn): update{_fn}, type{ptype}, position{pos} {};
  
        public:
            PieceType type{PieceType::_INVALID};
            board::bitmap::Squares position{0};

            board::bitmap::Squares observed{0}; // All "seen" squares and pieces
            board::bitmap::Squares attackable{0}; // All squares the piece might move to (not considering checkmate)
            
            Piece(): Piece(PieceType::_INVALID, 0, [](piece::Piece&, const board::Board&, sqrs, sqrs){}) {};

            void update_observed_and_attackable(const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies) {
                assert(this->type != PieceType::_INVALID);
                update(*this, board, pos_all, pos_hostile_armies);
            };

            void mark_as_dead() {
                position = 0;
                observed = 0;
                attackable = 0;
            }

            bool is_alive() const {
                return position > 0;
            }

            bool operator == (const Piece& other) const {
                return position == other.position && observed == other.observed && attackable == other.attackable && type == other.type;
            }
    };

}
